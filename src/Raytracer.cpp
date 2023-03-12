#include "Camera.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <thread>

namespace {
auto make_random_scene() noexcept
{
    auto scene = Scene();

    const auto ground_material = std::make_shared<Lambertian>(sf::Vector3f(0.5, 0.5, 0.5));
    scene.push_back(std::make_unique<Sphere>(sf::Vector3f(0, -1000, 0), 1000.f, ground_material));

    for (int i = -11; i < 11; ++i) {
        for (int j = -11; j < 11; ++j) {
            const auto center = sf::Vector3f(float(i) + random_float(0, 0.9f), 0.2f, float(j) + random_float(0, 0.9f));
            if ((center - sf::Vector3f(4, 0.2f, 0)).length() <= 0.9f)
                continue;

            auto material = std::shared_ptr<Material>();
            if (std::bernoulli_distribution(0.8)(rng())) {
                // diffuse
                const auto albedo = random_vector(0, 1).cwiseMul(random_vector(0, 1));
                material = std::make_shared<Lambertian>(albedo);
            } else if (std::bernoulli_distribution(0.95)(rng())) {
                // metal
                const auto albedo = random_vector(0.5f, 1);
                const auto fuzz = random_float(0, 0.5f);
                material = std::make_shared<Metal>(albedo, fuzz);
            } else {
                // glass
                material = std::make_shared<Dielectric>(1.5f);
            }

            scene.push_back(std::make_unique<Sphere>(center, 0.2f, material));
        }
    }

    scene.push_back(std::make_unique<Sphere>(sf::Vector3f(0, 1, 0), 1.f, std::make_shared<Dielectric>(1.5f)));
    scene.push_back(std::make_unique<Sphere>(
        sf::Vector3f(-4, 1, 0), 1.f, std::make_shared<Lambertian>(sf::Vector3f(0.4f, 0.2f, 0.1f))));
    scene.push_back(std::make_unique<Sphere>(
        sf::Vector3f(4.f, 1.f, 0.f), 1.f, std::make_shared<Metal>(sf::Vector3f(0.7f, 0.6f, 0.5f), 0.f)));

    return scene;
}

constexpr auto to_color(sf::Vector3f vector, const int samples_per_pixel) noexcept
{
    const auto adjust = [samples_per_pixel](const float channel) {
        return uint8_t(255 * std::clamp(std::sqrt(channel / float(samples_per_pixel)), 0.f, 1.f));
    };

    const auto r = adjust(vector.x);
    const auto g = adjust(vector.y);
    const auto b = adjust(vector.z);
    return sf::Color(r, g, b);
}

auto ray_color(const Ray& ray, const int depth) noexcept -> sf::Vector3f
{
    static const auto scene = make_random_scene();

    if (depth == 0)
        return {};

    if (const auto maybe_hit_record = hit(scene, ray, 0.001f, std::numeric_limits<float>::infinity())) {
        if (const auto result = maybe_hit_record->material->scatter(ray, *maybe_hit_record)) {
            const auto& [attenuation, scattered] = *result;
            return attenuation.cwiseMul(ray_color(scattered, depth - 1));
        }
    }

    const auto unit_direction = ray.direction().normalized();
    const auto t = 0.5f * (unit_direction.y + 1);
    return (1 - t) * sf::Vector3f(1, 1, 1) + t * sf::Vector3f(0.5f, 0.7f, 1.f);
}
}

int main()
{
    // Define constants
    constexpr auto aspect_ratio = 3.f / 2;
    constexpr auto image_height = 360;
    constexpr auto image_width = int(aspect_ratio * image_height);

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, image_width>, image_height>>();
    auto& pixels = *pixels_allocation;

    // Make camera
    const auto look_from = sf::Vector3f(13, 2, 3);
    const auto look_at = sf::Vector3f(0, 0, 0);
    const auto vup = sf::Vector3f(0, 1, 0);
    const auto aperture = 0.1f;
    const auto focus_distance = 10.f;
    const auto camera = Camera(look_from, look_at, vup, sf::degrees(20), aspect_ratio, aperture, focus_distance);

    // Set up rendering logic
    const auto render_rows = [&pixels, camera](const size_t start, const size_t end) noexcept {
        for (size_t i = start; i < end; ++i) {
            for (size_t j = 0; j < image_width; ++j) {
                static constexpr auto samples_per_pixel = 50;

                auto color = sf::Vector3f();
                for (size_t sample = 0; sample < samples_per_pixel; ++sample) {
                    const auto u = (random_float(0, 1) + float(j)) / (image_width - 1);
                    const auto v = (random_float(0, 1) + float(image_height - i)) / (image_height - 1);
                    const auto ray = camera.get_ray(u, v);
                    static constexpr auto max_depth = 10;
                    color += ray_color(ray, max_depth);
                }

                pixels[i][j] = to_color(color, samples_per_pixel);
            }
        }
    };

    // Render
    auto threads = std::vector<std::thread>(std::thread::hardware_concurrency());
    const auto rows_per_thread = float(image_height) / float(threads.size());
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i]
            = std::thread(render_rows, size_t(float(i) * rows_per_thread), size_t((float(i) + 1) * rows_per_thread));

    // Draw
    auto window = sf::RenderWindow(sf::VideoMode({ image_width, image_height }), "Raytracer");
    window.setFramerateLimit(15);
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

        auto image = sf::Image();
        image.create({ image_width, image_height }, reinterpret_cast<uint8_t*>(pixels.data()));
        auto texture = sf::Texture();
        if (!texture.loadFromImage(image))
            throw std::runtime_error("Failed to load texture");
        const auto sprite = sf::Sprite(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    for (auto& thread : threads)
        thread.join();
}
