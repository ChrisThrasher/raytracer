#include "Camera.hpp"

namespace {
constexpr auto aspect_ratio = 16.f / 9.f;
constexpr auto viewport_height = 2.f;
constexpr auto viewport_width = aspect_ratio * viewport_height;
constexpr auto focal_length = 1.f;
}

Camera::Camera()
    : m_horizontal(viewport_width, 0, 0)
    , m_vertical(0, viewport_height, 0)
    , m_lower_left_corner(m_origin - m_horizontal / 2.f - m_vertical / 2.f - sf::Vector3f(0, 0, focal_length))
{
}

auto Camera::get_ray(const float u, const float v) -> Ray
{
    return { m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin };
}
