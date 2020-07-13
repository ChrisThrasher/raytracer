#pragma once

#include "Ray.h"
#include "Utilities.h"
#include "Vec3.h"

class Camera
{
    const Point3 m_origin{};
    Point3 m_lower_left_corner{};
    Vec3 m_horizontal{};
    Vec3 m_vertical{};
    const Vec3 m_w{};
    const Vec3 m_u{};
    const Vec3 m_v{};
    const double m_lens_radius{0.0};

public:
    Camera(const Point3& lookfrom,
           const Point3& lookat,
           const Vec3& vup,
           const double vfov,
           const double aspect_ratio,
           const double aperture,
           const double focus_distance)
        : m_origin(lookfrom)
        , m_w(UnitVector(lookfrom - lookat))
        , m_u(UnitVector(Cross(vup, m_w)))
        , m_v(Cross(m_w, m_u))
        , m_lens_radius(aperture / 2)
    {
        const auto theta = DegreesToRadians(vfov);
        const auto h = std::tan(theta / 2);
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;

        m_horizontal = focus_distance * viewport_width * m_u;
        m_vertical = focus_distance * viewport_height * m_v;
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - focus_distance * m_w;
    }

    auto GetRay(const double s, const double t) const
    {
        const auto rd = m_lens_radius * RandomInUnitDisk();
        const auto offset = m_u * rd.X() + m_v * rd.Y();
        return Ray(m_origin + offset,
                   m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset);
    }
};
