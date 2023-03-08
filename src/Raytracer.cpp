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
#include <chrono>
#include <iostream>
#include <memory>

namespace {
constexpr auto to_color(sf::Vector3f vector, const int samples_per_pixel)
{
    const auto adjust = [samples_per_pixel](const float channel) {
        return uint8_t(255 * std::clamp(std::sqrtf(channel / float(samples_per_pixel)), 0.f, 1.f));
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
}

int main()
{
    constexpr auto aspect_ratio = 16.f / 9.f;
    constexpr auto image_width = 400;
    constexpr auto image_height = int(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100;
    constexpr auto max_depth = 50;

    const auto ground_material = std::make_shared<Lambertian>(sf::Vector3f(0.8f, 0.8f, 0.0f));
    const auto center_material = std::make_shared<Lambertian>(sf::Vector3f(0.7f, 0.3f, 0.3f));
    const auto left_material = std::make_shared<Metal>(sf::Vector3f(0.8f, 0.8f, 0.8f), 0.3f);
    const auto right_material = std::make_shared<Metal>(sf::Vector3f(0.8f, 0.6f, 0.2f), 1.f);

    auto world = HittableList();
    world.add(std::make_unique<Sphere>(sf::Vector3f(0.0, -100.5, -1.0), 100.0, ground_material));
    world.add(std::make_unique<Sphere>(sf::Vector3f(0.0, 0.0, -1.0), 0.5, center_material));
    world.add(std::make_unique<Sphere>(sf::Vector3f(-1.0, 0.0, -1.0), 0.5, left_material));
    world.add(std::make_unique<Sphere>(sf::Vector3f(1.0, 0.0, -1.0), 0.5, right_material));

    auto camera = Camera();

    auto window = sf::RenderWindow(sf::VideoMode({ image_width, image_height }), "Raytracer");
    window.setFramerateLimit(30);

    auto font = sf::Font();
    if (!font.loadFromFile(FONT_PATH / std::filesystem::path("font.ttf")))
        throw std::runtime_error("Failed to load font");

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, image_width>, image_height>>();
    auto& pixels = *pixels_allocation;

    const auto start_of_rendering = std::chrono::steady_clock::now();

    for (size_t i = 0; i < image_height; ++i) {
        std::cout << "\rProcessing line " << i + 1 << " of " << image_height << std::flush;
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
    }
    std::cout << std::endl;

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
