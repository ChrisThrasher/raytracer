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
    Camera(const double vfov, const double aspect_ratio)
    {
        const auto theta = DegreesToRadians(vfov);
        const auto h = std::tan(theta / 2);
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;
        const auto focal_length = 1.0;

        m_origin = Point3(0, 0, 0);
        m_horizontal = Vec3(viewport_width, 0, 0);
        m_vertical = Vec3(0, viewport_height, 0);
        m_lower_left_corner =
            m_origin - m_horizontal / 2 - m_vertical / 2 - Vec3(0, 0, focal_length);
    }

    constexpr auto GetRay(const double u, const double v) const
    {
        return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
    }
};
