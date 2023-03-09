#include "Camera.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

namespace {
constexpr auto to_color(sf::Vector3f vector, const int samples_per_pixel)
{
    const auto adjust = [samples_per_pixel](const float channel) {
        return uint8_t(255 * std::clamp(std::sqrt(channel / float(samples_per_pixel)), 0.f, 1.f));
    };

    const auto r = adjust(vector.x);
    const auto g = adjust(vector.y);
    const auto b = adjust(vector.z);
    return sf::Color(r, g, b);
}

auto ray_color(const Ray& ray, const Hittable& world, const int depth) -> sf::Vector3f
{
    assert(depth >= 0);
    if (depth == 0)
        return {};

    if (const auto maybe_hit_record = world.hit(ray, 0.001f, std::numeric_limits<float>::infinity())) {
        if (const auto result = maybe_hit_record->material->scatter(ray, *maybe_hit_record)) {
            const auto& [attenuation, scattered] = *result;
            return attenuation.cwiseMul(ray_color(scattered, world, depth - 1));
        }
    }

    const auto unit_direction = ray.direction().normalized();
    const auto t = 0.5f * (unit_direction.y + 1);
    return (1 - t) * sf::Vector3f(1, 1, 1) + t * sf::Vector3f(0.5f, 0.7f, 1.f);
}

auto make_random_scene()
{
    auto world = HittableList();

    const auto ground_material = std::make_shared<Lambertian>(sf::Vector3f(0.5, 0.5, 0.5));
    world.add(std::make_unique<Sphere>(sf::Vector3f(0, -1000, 0), 1000, ground_material));

    for (int i = -11; i < 11; ++i) {
        for (int j = -11; j < 11; ++j) {
            const auto choose_material = random_float(0, 1);
            const auto center
                = sf::Vector3f(float(i) + 0.9f * random_float(0, 1), 0.2f, float(j) + 0.9f + random_float(0, 1));

            if ((center - sf::Vector3f(4, 0.2f, 0)).length() > 0.9f) {
                if (choose_material < 0.8) {
                    // diffuse
                    const auto albedo = random_vector(0, 1).cwiseMul(random_vector(0, 1));
                    const auto material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_unique<Sphere>(center, 0.2, material));
                } else if (choose_material < 0.95) {
                    // metal
                    const auto albedo = random_vector(0.5f, 1);
                    const auto fuzz = random_float(0, 0.5f);
                    const auto material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_unique<Sphere>(center, 0.2, material));
                } else {
                    // glass
                    const auto material = std::make_shared<Dielectric>(1.5f);
                    world.add(std::make_unique<Sphere>(center, 0.2, material));
                }
            }
        }
    }

    world.add(std::make_unique<Sphere>(sf::Vector3f(0, 1, 0), 1, std::make_shared<Dielectric>(1.5f)));
    world.add(std::make_unique<Sphere>(
        sf::Vector3f(-4, 1, 0), 1, std::make_shared<Lambertian>(sf::Vector3f(0.4f, 0.2f, 0.1f))));
    world.add(
        std::make_unique<Sphere>(sf::Vector3f(4, 1, 0), 1, std::make_shared<Metal>(sf::Vector3f(0.7f, 0.6f, 0.5f), 0)));

    return world;
}
}

int main()
{
    constexpr auto aspect_ratio = 3.f / 2;
    constexpr auto image_width = 200;
    constexpr auto image_height = int(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100;
    constexpr auto max_depth = 50;

    const auto world = make_random_scene();

    const auto look_from = sf::Vector3f(13, 2, 3);
    const auto look_at = sf::Vector3f(0, 0, 0);
    const auto vup = sf::Vector3f(0, 1, 0);
    const auto focus_distance = 10.f;
    const auto aperture = 0.1f;
    auto camera = Camera(look_from, look_at, vup, sf::degrees(20), aspect_ratio, aperture, focus_distance);

    auto window = sf::RenderWindow(sf::VideoMode({ image_width, image_height }), "Raytracer");
    window.setFramerateLimit(30);

    auto font = sf::Font();
    if (!font.loadFromFile(FONT_PATH / std::filesystem::path("font.ttf")))
        throw std::runtime_error("Failed to load font");

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, image_width>, image_height>>();
    auto& pixels = *pixels_allocation;

    auto rendered_row_count = std::atomic(0);

    const auto render_rows
        = [&pixels, camera, &world, &rendered_row_count](const size_t start, const size_t end) noexcept {
              for (size_t i = start; i < end; ++i) {
                  for (size_t j = 0; j < image_width; ++j) {
                      auto color = sf::Vector3f();
                      for (size_t sample = 0; sample < samples_per_pixel; ++sample) {
                          const auto u = (random_float(0, 1) + float(j)) / (image_width - 1);
                          const auto v = (random_float(0, 1) + float(image_height - i)) / (image_height - 1);
                          const auto ray = camera.get_ray(u, v);
                          color += ray_color(ray, world, max_depth);
                      }

                      pixels[i][j] = to_color(color, samples_per_pixel);
                  }

                  ++rendered_row_count;
              }
          };

    const auto start_of_rendering = std::chrono::steady_clock::now();

    auto rendering = std::atomic(true);
    auto threads = std::vector<std::thread>(std::thread::hardware_concurrency());
    const auto rows_per_thread = float(image_height) / float(threads.size());
    auto status_thread = std::thread([&rendered_row_count, &rendering]() {
        while (rendering) {
            std::cout << "\rRendered " << rendered_row_count << " of " << image_height << " rows" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << std::endl;
    });
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i]
            = std::thread(render_rows, size_t(float(i) * rows_per_thread), size_t((float(i) + 1) * rows_per_thread));
    for (auto& thread : threads)
        thread.join();
    if (rendered_row_count != image_height)
        throw std::runtime_error("Rendered row count of " + std::to_string(rendered_row_count) + " should be "
                                 + std::to_string(image_height));
    rendering = false;
    status_thread.join();

    const auto rendering_time
        = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_of_rendering);

    auto image = sf::Image();
    image.create({ image_width, image_height }, reinterpret_cast<uint8_t*>(pixels.data()));
    auto texture = sf::Texture();
    if (!texture.loadFromImage(image))
        throw std::runtime_error("Failed to load texture");
    const auto sprite = sf::Sprite(texture);

    auto text = sf::Text(std::to_string(rendering_time.count()) + " ms", font, 28);
    text.setPosition({ 5, 0 });
    text.setOutlineThickness(2.f);

    while (window.isOpen()) {
        for (auto event = sf::Event(); window.pollEvent(event);) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        window.draw(sprite);
        window.draw(text);
        window.display();
    }
}
