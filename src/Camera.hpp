#pragma once

#include "Ray.hpp"

#include <SFML/System/Angle.hpp>

class Camera {
    sf::Vector3f m_origin;
    sf::Vector3f m_horizontal;
    sf::Vector3f m_vertical;
    sf::Vector3f m_lower_left_corner;

public:
    Camera(const sf::Vector3f& look_from,
           const sf::Vector3f& look_at,
           const sf::Vector3f& vup,
           sf::Angle fov,
           float aspect_ratio);

    [[nodiscard]] auto get_ray(float s, float t) const -> Ray;
};
