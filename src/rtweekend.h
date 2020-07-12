#pragma once

#include <limits>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline auto degrees_to_radians(const double degrees)
{
    return degrees * pi / 180.0;
}

inline auto random_double()
{
    static auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);
    static auto generator = std::mt19937();
    return distribution(generator);
}

inline auto random_double(const double min, const double max)
{
    return std::clamp(random_double(), min, max);
}
