#include "Hittable.hpp"

void set_face_normal(HitRecord& hit_record, const Ray& ray, const sf::Vector3f& outward_normal) noexcept
{
    hit_record.front_face = ray.direction().dot(outward_normal) < 0;
    hit_record.normal = hit_record.front_face ? outward_normal : -outward_normal;
}

auto hit(const Scene& scene, const Ray& ray, const float t_min, const float t_max) noexcept -> std::optional<HitRecord>
{
    auto hit_record = HitRecord();
    auto hit_something = false;
    auto closest_yet = t_max;

    for (const auto& object : scene) {
        if (const auto maybe_hit_record = object->hit(ray, t_min, closest_yet)) {
            hit_something = true;
            closest_yet = maybe_hit_record->t;
            hit_record = *maybe_hit_record;
        }
    }

    if (hit_something)
        return hit_record;
    return {};
}
