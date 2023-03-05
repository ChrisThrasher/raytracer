#include "Ray.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <iostream>
#include <memory>

namespace {
constexpr float hit_sphere(const sf::Vector3f& center, float radius, const Ray& ray)
{
    const auto oc = ray.origin() - center;
    const auto a = ray.direction().lengthSq();
    const auto b = 2.f * oc.dot(ray.direction());
    const auto c = oc.lengthSq() - radius * radius;
    const auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1;
    else
        return (-b - std::sqrtf(discriminant)) / (2 * a);
}

constexpr auto to_color(const sf::Vector3f& vector)
{
    return sf::Color(uint8_t(255 * vector.x), uint8_t(255 * vector.y), uint8_t(255 * vector.z));
}

constexpr auto ray_color(const Ray& ray)
{
    auto t = hit_sphere({ 0, 0, -1 }, 0.5f, ray);
    if (t > 0) {
        const auto n = (ray.at(t) - sf::Vector3f(0, 0, -1)).normalized();
        return to_color(0.5f * sf::Vector3f(n.x + 1, n.y + 1, n.z + 1));
    }
    const auto unit_direction = ray.direction().normalized();
    t = 0.5f * (unit_direction.y + 1);
    return to_color((1 - t) * sf::Vector3f(1, 1, 1) + t * sf::Vector3f(0.5f, 0.7f, 1.f));
}
}

int main()
{
    constexpr auto aspect_ratio = 16.f / 9.f;
    constexpr auto image_width = 400;
    constexpr auto image_height = int(image_width / aspect_ratio);

    constexpr auto viewport_height = 2.f;
    constexpr auto viewport_width = aspect_ratio * viewport_height;
    constexpr auto focal_length = 1.f;

    constexpr auto origin = sf::Vector3f();
    constexpr auto horizontal = sf::Vector3f(viewport_width, 0, 0);
    constexpr auto vertical = sf::Vector3f(0, viewport_height, 0);
    constexpr auto lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - sf::Vector3f(0, 0, focal_length);

    auto window = sf::RenderWindow(sf::VideoMode({ image_width, image_height }), "Raytracer");
    window.setFramerateLimit(30);

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, image_width>, image_height>>();
    auto& pixels = *pixels_allocation;

    const auto start_of_rendering = std::chrono::steady_clock::now();

    for (size_t i = 0; i < image_height; ++i) {
        for (size_t j = 0; j < image_width; ++j) {
            const auto u = float(j) / (image_width - 1);
            const auto v = float(image_height - i) / (image_height - 1);
            const auto ray = Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            pixels[i][j] = ray_color(ray);
        }
    }

    const auto rendering_time
        = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_of_rendering);
    std::cout << "Rendered in " << rendering_time.count() << " ms" << std::endl;

    auto image = sf::Image();
    image.create({ image_width, image_height }, reinterpret_cast<uint8_t*>(pixels.data()));
    auto texture = sf::Texture();
    if (!texture.loadFromImage(image))
        throw std::runtime_error("Failed to load texture");
    const auto sprite = sf::Sprite(texture);

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
        window.display();
    }
}
