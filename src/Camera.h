#pragma once

#include "Ray.h"
#include "Utilities.h"
#include "Vec3.h"

class Camera
{
    Point3 m_origin{};
    Point3 m_lower_left_corner{};
    Vec3 m_horizontal{};
    Vec3 m_vertical{};

public:
    Camera(const Point3& lookfrom,
           const Point3& lookat,
           const Vec3& vup,
           const double vfov,
           const double aspect_ratio)
    {
        const auto theta = DegreesToRadians(vfov);
        const auto h = std::tan(theta / 2);
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;

        const auto w = UnitVector(lookfrom - lookat);
        const auto u = UnitVector(Cross(vup, w));
        const auto v = Cross(w, u);

        m_origin = lookfrom;
        m_horizontal = viewport_width * u;
        m_vertical = viewport_height * v;
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - w;
    }

    constexpr auto GetRay(const double u, const double v) const
    {
        return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
    }
};
