#include "Ray.hpp"

Ray::Ray(const sf::Vector3f& origin, const sf::Vector3f& direction)
    : m_origin(origin)
    , m_direction(direction)
{
}
