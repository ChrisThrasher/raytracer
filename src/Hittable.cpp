#include "Hittable.hpp"

void set_face_normal(Hit& hit, const Ray& ray, const sf::Vector3f& outward_normal) noexcept
{
    hit.front_face = ray.direction().dot(outward_normal) < 0;
    hit.normal = hit.front_face ? outward_normal : -outward_normal;
}
