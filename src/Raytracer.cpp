#include "Camera.hpp"
#include "Material.hpp"
#include "Random.hpp"
#include "Sphere.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <atomic>
#include <iostream>
#include <thread>

using Scene = std::vector<Sphere>;

namespace {
[[nodiscard]] auto hit(const Scene& scene, const Ray& ray, const float t_min, const float t_max) noexcept
{
    auto hit = std::optional<Hit>();
    auto closest_yet = t_max;

    for (const auto& object : scene) {
        if (const auto maybe_hit = object.hit(ray, t_min, closest_yet)) {
            closest_yet = maybe_hit->t;
            hit = maybe_hit;
        }
    }

    return hit;
}

[[nodiscard]] auto make_scene() noexcept
{
    auto scene = Scene();

    // Add ground
    scene.emplace_back(sf::Vector3f(0, -1000, 0), 1000.f, Lambertian { { 0.3f, 0.6f, 0.1f } });

    // Add fixed large spheres
    scene.emplace_back(sf::Vector3f(-4, 2, 0), 2.f, Lambertian { { 0.2f, 0.3f, 0.7f } });
    scene.emplace_back(sf::Vector3f(-0.5f, 1.5, 1), 1.5f, Metal { { 0.7f, 0.4f, 0.3f }, 1.f });
    scene.emplace_back(sf::Vector3f(1.25f, 1, 1.5f), 1.f, Metal { { 0.7f, 0.6f, 0.5f }, 0.f });
    scene.emplace_back(sf::Vector3f(3, 0.75f, 3), 0.75f, Metal { { 0.8f, 0.2f, 0.1f }, 0.5f });
    scene.emplace_back(sf::Vector3f(4, 0.5f, 5), 0.5f, Metal { { 0.8f, 0.7f, 0.1f }, 0.f });
    scene.emplace_back(sf::Vector3f(2, 0.5, 6), 0.5f, Dielectric { 1.5f });

    return scene;
}

[[nodiscard]] auto to_color(sf::Vector3f vector, const int samples_per_pixel) noexcept
{
    const auto adjust = [samples_per_pixel](const float channel) noexcept {
        return uint8_t(255 * std::clamp(std::sqrt(channel / float(samples_per_pixel)), 0.f, 1.f));
    };

    const auto r = adjust(vector.x);
    const auto g = adjust(vector.y);
    const auto b = adjust(vector.z);
    return sf::Color(r, g, b);
}

[[nodiscard]] auto trace_ray(const Scene& scene, const Ray& ray, const int depth) noexcept
{
    assert(depth >= 0);
    if (depth == 0)
        return sf::Vector3f();

    if (const auto maybe_hit = hit(scene, ray, 0.001f, std::numeric_limits<float>::infinity())) {
        if (const auto result = scatter(*maybe_hit->material, ray, *maybe_hit)) {
            const auto& [attenuation, scattered] = *result;
            return attenuation.cwiseMul(trace_ray(scene, scattered, depth - 1));
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
    constexpr auto image_height = 640;
    constexpr auto image_width = int(aspect_ratio * image_height);

    // Make image
    auto image = sf::Image();
    image.create({ image_width, image_height });

    // Make scene
    const auto scene = make_scene();

    // Make camera
    const auto camera = []() {
        const auto look_from = sf::Vector3f(5, 1.25f, 10);
        const auto look_at = sf::Vector3f(0, 1, 0);
        const auto vup = sf::Vector3f(0, 1, 0);
        const auto fov = sf::degrees(40);
        const auto aperture = 0.04f;
        const auto focus_distance = (look_at - look_from).length();
        return Camera(look_from, look_at, vup, fov, aspect_ratio, aperture, focus_distance);
    }();

    // Set up rendering logic
    const auto render_rows = [&image, &scene, camera](const size_t thread_count) noexcept {
        // Tuning parameters
        static constexpr auto samples_per_pixel = 100;
        static constexpr auto max_depth = 10;

        static auto current_row = std::atomic<unsigned>(0);
        static auto completed_threads = std::atomic<size_t>(0);
        static auto now = std::chrono::steady_clock::now();

        // Render current row
        for (unsigned i = current_row++; i < image_height; i = current_row++) {
            for (unsigned j = 0; j < image_width; ++j) {

                auto color = sf::Vector3f();
                for (size_t sample = 0; sample < samples_per_pixel; ++sample) {
                    const auto u = (random_float(0, 1) + float(j)) / (image_width + 1);
                    const auto v = (random_float(0, 1) + float(image_height - i)) / (image_height + 1);
                    const auto ray = camera.get_ray(u, v);
                    color += trace_ray(scene, ray, max_depth);
                }

                image.setPixel({ j, i }, to_color(color, samples_per_pixel));
            }
        }

        // Return if not the final thread to complete rendering
        if (++completed_threads < thread_count)
            return;

        // Print elapased time
        const auto elapsed
            = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - now);
        std::cout << "Render time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;
    };

    // Start rendering
    auto threads = std::vector<std::thread>(std::thread::hardware_concurrency());
    for (auto& thread : threads)
        thread = std::thread(render_rows, threads.size());

    // Draw
    auto window = sf::RenderWindow(sf::VideoMode({ image_width, image_height }), "Raytracer");
    window.setFramerateLimit(15);
    while (window.isOpen()) {
        for (auto event = sf::Event(); window.pollEvent(event);) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                break;
            default:
                break;
            }
        }

        auto texture = sf::Texture();
        if (!texture.loadFromImage(image))
            throw std::runtime_error("Failed to load texture");

        window.clear();
        window.draw(sf::Sprite(texture));
        window.display();
    }

    for (auto& thread : threads)
        thread.join();
}
