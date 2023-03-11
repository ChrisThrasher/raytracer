#pragma once

#include <SFML/System/Vector3.hpp>

class Ray {
    sf::Vector3f m_origin;
    sf::Vector3f m_direction;

public:
    constexpr Ray(const sf::Vector3f& origin, const sf::Vector3f& direction) noexcept
        : m_origin(origin)
        , m_direction(direction)
    {
    }

    [[nodiscard]] constexpr auto origin() const { return m_origin; }
    [[nodiscard]] constexpr auto direction() const { return m_direction; }
    [[nodiscard]] constexpr auto at(float t) const { return m_origin + t * m_direction; }
};
