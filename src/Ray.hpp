#pragma once

#include <SFML/System/Vector3.hpp>

class Ray {
    sf::Vector3f m_origin;
    sf::Vector3f m_direction;

public:
    Ray() = default;
    Ray(const sf::Vector3f& origin, const sf::Vector3f& direction);

    [[nodiscard]] auto origin() const { return m_origin; }
    [[nodiscard]] auto direction() const { return m_direction; }
    [[nodiscard]] auto at(float t) const { return m_origin + t * m_direction; }
};
