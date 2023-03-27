#include "Hittable.hpp"

void set_face_normal(HitRecord& hit_record, const Ray& ray, const sf::Vector3f& outward_normal) noexcept
{
    hit_record.front_face = ray.direction().dot(outward_normal) < 0;
    hit_record.normal = hit_record.front_face ? outward_normal : -outward_normal;
}
