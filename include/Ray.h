#pragma once

#include "Vec3.h"

class Ray {
    Point3 m_origin {};
    Vec3 m_direction {};

public:
    constexpr Ray() = default;
    constexpr Ray(const Point3& origin, const Vec3& direction)
        : m_origin(origin)
        , m_direction(direction)
    {
    }

    constexpr auto Origin() const { return m_origin; }
    constexpr auto Direction() const { return m_direction; }

    constexpr auto At(const double t) const { return m_origin + t * m_direction; }
};
