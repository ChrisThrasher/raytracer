#pragma once

#include "Ray.hpp"

class Camera {
    sf::Vector3f m_origin;
    sf::Vector3f m_horizontal;
    sf::Vector3f m_vertical;
    sf::Vector3f m_lower_left_corner;

public:
    Camera();

    auto get_ray(float u, float v) -> Ray;
};
