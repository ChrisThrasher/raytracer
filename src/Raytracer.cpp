#include "Camera.hpp"
#include "Material.hpp"
#include "Random.hpp"
#include "Sphere.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <type_traits>

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

template <class T>
struct counting_iterator {
    static_assert(std::is_integral_v<T>, "T must be an integral type");
    using difference_type = T; // std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    constexpr T operator*() const noexcept { return m_value; }

    constexpr counting_iterator& operator++() noexcept
    {
        ++m_value;
        return *this;
    }
    constexpr counting_iterator operator++(int) noexcept
    {
        T rv { m_value };
        ++m_value;
        return rv;
    }
    constexpr counting_iterator& operator--() noexcept
    {
        --m_value;
        return *this;
    }
    constexpr counting_iterator operator--(int) noexcept
    {
        T rv { m_value };
        --m_value;
        return rv;
    }

    constexpr counting_iterator& operator+=(difference_type n) noexcept
    {
        m_value += n;
        return *this;
    }
    constexpr counting_iterator& operator-=(difference_type n) noexcept
    {
        m_value -= n;
        return *this;
    }

    friend constexpr counting_iterator operator+(const counting_iterator& lhs, const difference_type n) noexcept
    {
        return counting_iterator { lhs } += n;
    }
    friend constexpr counting_iterator operator+(const difference_type n, const counting_iterator& rhs) noexcept
    {
        return counting_iterator { rhs } += n;
    }
    friend constexpr difference_type operator-(const counting_iterator& lhs, const counting_iterator& rhs) noexcept
    {
        return lhs.m_value - rhs.m_value;
    }

    constexpr value_type operator[](difference_type n) noexcept { return m_value + n; }

    constexpr bool operator==(const counting_iterator& other) const noexcept { return m_value == other.m_value; }
    constexpr bool operator!=(const counting_iterator& other) const noexcept { return m_value != other.m_value; }
    constexpr bool operator<(const counting_iterator& other) const noexcept { return m_value < other.m_value; }
    constexpr bool operator>(const counting_iterator& other) const noexcept { return m_value > other.m_value; }
    constexpr bool operator<=(const counting_iterator& other) const noexcept { return m_value <= other.m_value; }
    constexpr bool operator>=(const counting_iterator& other) const noexcept { return m_value >= other.m_value; }

    T m_value;
};

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
    const auto render_rows = [&image, &scene, camera](const unsigned row) noexcept {
        // Tuning parameters
        static constexpr auto samples_per_pixel = 100;
        static constexpr auto max_depth = 10;

        // Render current row
        for (unsigned j = 0; j < image_width; ++j) {

            auto color = sf::Vector3f();
            for (size_t sample = 0; sample < samples_per_pixel; ++sample) {
                const auto u = (random_float(0, 1) + float(j)) / (image_width + 1);
                const auto v = (random_float(0, 1) + float(image_height - row)) / (image_height + 1);
                const auto ray = camera.get_ray(u, v);
                color += trace_ray(scene, ray, max_depth);
            }

            image.setPixel({ j, row }, to_color(color, samples_per_pixel));
        }
    };

    const auto now = std::chrono::steady_clock::now();

    // Start rendering

#if RT_ROW_METHOD == 0
    // Using a vector for storing row numbers:
    std::vector<unsigned> rows(image_height);
    std::iota(rows.begin(), rows.end(), 0u);
    std::for_each(std::execution::RT_EXECUTION_POLICY, rows.begin(), rows.end(), render_rows);
#elif RT_ROW_METHOD == 1
    // Using a unique_ptr<unsigned[]> for storing row numbers:
    std::unique_ptr<unsigned[]> rows(new unsigned[image_height]); // no initialization
    std::iota(rows.get(), rows.get() + image_height, 0u);
    std::for_each(std::execution::RT_EXECUTION_POLICY, rows.get(), rows.get() + image_height, render_rows);
#elif RT_ROW_METHOD == 2
    // counting_iterator for row numbers - no setup needed
    std::for_each(std::execution::RT_EXECUTION_POLICY,
                  counting_iterator<unsigned> { 0u },
                  counting_iterator<unsigned> { image_height },
                  render_rows);
#else
    static_assert(false, "ROW_METHOD must be [0,2]");
#endif

    // Print elapased time
    const auto elapsed
        = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - now);
    std::cout << "Render time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;

    // Draw
    auto window = sf::RenderWindow(
        sf::VideoMode({ image_width, image_height }), "Raytracer", sf::Style::Default ^ sf::Style::Resize);
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
}
