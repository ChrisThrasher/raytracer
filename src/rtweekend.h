#pragma once

#include <limits>
#include <random>

constexpr auto infinity = std::numeric_limits<double>::infinity();
constexpr auto pi = 3.1415926535897932385;

constexpr auto DegreesToRadians(const double degrees) { return degrees * pi / 180.0; }

inline auto RandomDouble()
{
    static auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);
    static auto generator = std::mt19937();
    return distribution(generator);
}

inline auto RandomDouble(const double min, const double max)
{
    return std::clamp(RandomDouble(), min, max);
}
