#include "Utilities.hpp"

#include <array>

auto rng() -> std::mt19937&
{
    thread_local auto generator = []() {
        auto seed_data = std::array<int, std::mt19937::state_size>();
        auto random_device = std::random_device();
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(random_device));
        auto sequence = std::seed_seq(seed_data.begin(), seed_data.end());
        return std::mt19937(sequence);
    }();
    return generator;
}

auto random_float(const float min, const float max) -> float { return std::uniform_real_distribution(min, max)(rng()); }

auto random_unit_vector() -> sf::Vector3f
{
    auto vector = sf::Vector3f();
    do {
        vector = { random_float(-1, 1), random_float(-1, 1), random_float(-1, 1) };
    } while (vector.lengthSq() >= 1);
    return vector.normalized();
}

auto is_near_zero(const sf::Vector3f& vector) -> bool
{
    constexpr auto epsilon = 1e-9f;
    return vector.lengthSq() < epsilon;
}
