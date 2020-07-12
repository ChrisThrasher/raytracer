#pragma once

#include "vec3.h"

class ray
{
    point3 orig{};
    vec3 dir{};

public:
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    auto origin() const { return orig; }
    auto direction() const { return dir; }

    auto at(const double t) const
    {
        return orig + t * dir;
    }
};
