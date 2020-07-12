#pragma once

#include "vec3.h"

class Ray
{
    point3 orig{};
    vec3 dir{};

public:
    constexpr Ray() = default;
    constexpr Ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    constexpr auto Origin() const { return orig; }
    constexpr auto Direction() const { return dir; }

    constexpr auto At(const double t) const
    {
        return orig + t * dir;
    }
};
