#include "Hittable.hpp"

auto hit(const std::vector<std::unique_ptr<Hittable>>& world, const Ray& ray, const float t_min, const float t_max)
    -> std::optional<HitRecord>
{
    auto hit_record = HitRecord();
    auto hit_something = false;
    auto closest_yet = t_max;

    for (const auto& object : world) {
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