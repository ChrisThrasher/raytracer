#include "Camera.hpp"
#include "Utilities.hpp"

#include <cmath>

Camera::Camera(const sf::Vector3f& look_from,
               const sf::Vector3f& look_at,
               const sf::Vector3f& vup,
               const sf::Angle fov,
               const float aspect_ratio,
               const float aperture,
               const float focus_distance)
    : m_origin(look_from)
    , m_w((look_from - look_at).normalized())
    , m_u(vup.cross(m_w).normalized())
    , m_v(m_w.cross(m_u))
    , m_lens_radius(aperture / 2)
{
    const auto half_height = std::tan(fov.asRadians() / 2);
    const auto viewport_height = 2 * half_height;
    const auto viewport_width = aspect_ratio * viewport_height;

    m_horizontal = focus_distance * viewport_width * m_u;
    m_vertical = focus_distance * viewport_height * m_v;
    m_lower_left_corner = m_origin - m_horizontal / 2.f - m_vertical / 2.f - focus_distance * m_w;
}

auto Camera::get_ray(const float s, const float t) const -> Ray
{
    const auto rd = m_lens_radius * random_vector_in_unit_disk();
    const auto offset = m_u * rd.x + m_v * rd.y;
    return { m_origin + offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset };
}
