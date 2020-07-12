#pragma once

#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline auto degrees_to_radians(const double degrees)
{
    return degrees * pi / 180.0;
}
