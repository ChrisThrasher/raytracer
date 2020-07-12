#pragma once

#include "vec3.h"

class Ray
{
    point3 m_orig{};
    Vec3 m_dir{};

public:
    constexpr Ray() = default;
    constexpr Ray(const point3& origin, const Vec3& direction)
        : m_orig(origin), m_dir(direction)
    {}

    constexpr auto Origin() const { return m_orig; }
    constexpr auto Direction() const { return m_dir; }

    constexpr auto At(const double t) const
    {
        return m_orig + t * m_dir;
    }
};
