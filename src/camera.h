#pragma once

class Camera
{
    point3 m_origin{};
    point3 m_lower_left_corner{};
    Vec3 m_horizontal{};
    Vec3 m_vertical{};

public:
    constexpr Camera()
    {
        constexpr auto aspect_ratio = 16.0 / 9.0;
        constexpr auto viewport_height = 2.0;
        constexpr auto viewport_width = aspect_ratio * viewport_height;
        constexpr auto focal_length = 1.0;

        m_origin = point3(0, 0, 0);
        m_horizontal = Vec3(viewport_width, 0.0, 0.0);
        m_vertical = Vec3(0.0, viewport_height, 0.0);
        m_lower_left_corner = m_origin
                          - m_horizontal / 2
                          - m_vertical / 2
                          - Vec3(0, 0, focal_length);
    }

    constexpr auto GetRay(const double u, const double v) const
    {
        return Ray(m_origin,
                   m_lower_left_corner
                   + u * m_horizontal
                   + v * m_vertical
                   - m_origin);
    }
};
