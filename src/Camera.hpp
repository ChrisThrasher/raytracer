#pragma once

#include "Ray.hpp"

#include <SFML/System/Angle.hpp>

class Camera {
    sf::Vector3f m_origin;
    sf::Vector3f m_horizontal;
    sf::Vector3f m_vertical;
    sf::Vector3f m_lower_left_corner;

public:
    Camera(sf::Angle fov, float aspect_ratio);

    auto get_ray(float u, float v) -> Ray;
};
