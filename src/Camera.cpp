#include "Camera.hpp"

#include <cmath>

Camera::Camera(const sf::Vector3f& look_from,
               const sf::Vector3f& look_at,
               const sf::Vector3f& vup,
               const sf::Angle fov,
               const float aspect_ratio)
{
    const auto half_height = std::tan(fov.asRadians() / 2);
    const auto viewport_height = 2 * half_height;
    const auto viewport_width = aspect_ratio * viewport_height;

    const auto w = (look_from - look_at).normalized();
    const auto u = vup.cross(w).normalized();
    const auto v = w.cross(u);

    m_origin = look_from;
    m_horizontal = viewport_width * u;
    m_vertical = viewport_height * v;
    m_lower_left_corner = m_origin - m_horizontal / 2.f - m_vertical / 2.f - w;
}

auto Camera::get_ray(const float s, const float t) const -> Ray
{
    return { m_origin, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin };
}
