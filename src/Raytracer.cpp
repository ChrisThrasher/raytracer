#include "Camera.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <memory>

namespace {
constexpr auto to_color(const sf::Vector3f& vector)
{
    return sf::Color(uint8_t(255 * vector.x), uint8_t(255 * vector.y), uint8_t(255 * vector.z));
}

auto ray_color(const Ray& ray, const Hittable& world)
{
    if (auto maybe_hit_record = world.hit(ray, 0, std::numeric_limits<float>::infinity()))
        return 0.5f * (maybe_hit_record->normal + sf::Vector3f(1, 1, 1));
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

    auto world = HittableList();
    world.add(std::make_shared<Sphere>(sf::Vector3f(0, 0, -1), 0.5f));
    world.add(std::make_shared<Sphere>(sf::Vector3f(0, -100.5, -1), 100));

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
        for (size_t j = 0; j < image_width; ++j) {
            auto color = sf::Vector3f();
            for (size_t sample = 0; sample < samples_per_pixel; ++sample) {
                const auto u = (random_float() + float(j)) / (image_width - 1);
                const auto v = (random_float() + float(image_height - i)) / (image_height - 1);
                const auto ray = camera.get_ray(u, v);
                color += ray_color(ray, world);
            }

            pixels[i][j] = to_color(color / float(samples_per_pixel));
        }
    }

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
