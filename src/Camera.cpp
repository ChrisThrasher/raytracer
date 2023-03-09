#include "Camera.hpp"

#include <cmath>

Camera::Camera(const sf::Angle fov, const float aspect_ratio)
{
    constexpr auto focal_length = 1.f;

    const auto half_height = std::tan(fov.asRadians() / 2);
    const auto viewport_height = 2 * half_height;
    const auto viewport_width = aspect_ratio * viewport_height;

    m_horizontal = { viewport_width, 0, 0 };
    m_vertical = { 0, viewport_height, 0 };
    m_lower_left_corner = { m_origin - m_horizontal / 2.f - m_vertical / 2.f - sf::Vector3f(0, 0, focal_length) };
}

auto Camera::get_ray(const float u, const float v) -> Ray
{
    return { m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin };
}
