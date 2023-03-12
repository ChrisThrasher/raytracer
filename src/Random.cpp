#include "Random.hpp"

#include <algorithm>
#include <array>

auto rng() noexcept -> std::minstd_rand&
{
    thread_local auto generator = []() {
        auto seed_data = std::array<std::random_device::result_type, 4>();
        auto random_device = std::random_device();
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(random_device));
        auto sequence = std::seed_seq(seed_data.begin(), seed_data.end());
        return std::minstd_rand(sequence);
    }();
    return generator;
}

auto random_float(const float min, const float max) noexcept -> float
{
    return std::uniform_real_distribution(min, max)(rng());
}

auto random_vector(float min, float max) noexcept -> sf::Vector3f
{
    return { random_float(min, max), random_float(min, max), random_float(min, max) };
}

auto random_vector_in_unit_sphere() noexcept -> sf::Vector3f
{
    auto vector = sf::Vector3f();
    do {
        vector = { random_float(-1, 1), random_float(-1, 1), random_float(-1, 1) };
    } while (vector.lengthSq() >= 1);
    return vector;
}

auto random_unit_vector() noexcept -> sf::Vector3f
{
    return sf::Vector3f(random_float(-1, 1), random_float(-1, 1), random_float(-1, 1)).normalized();
}

auto random_vector_in_hemisphere(const sf::Vector3f& normal) noexcept -> sf::Vector3f
{
    const auto in_unit_sphere = random_vector_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0)
        return in_unit_sphere;
    return -in_unit_sphere;
}

auto random_vector_in_unit_disk() noexcept -> sf::Vector3f
{
    auto vector = sf::Vector3f();
    do {
        vector = { random_float(-1, 1), random_float(-1, 1), 0 };
    } while (vector.lengthSq() >= 1);
    return vector;
}
