#pragma once

#include "vec3.h"

class ray
{
    point3 orig{};
    vec3 dir{};

public:
    constexpr ray() = default;
    constexpr ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    constexpr auto origin() const { return orig; }
    constexpr auto direction() const { return dir; }

    constexpr auto at(const double t) const
    {
        return orig + t * dir;
    }
};
