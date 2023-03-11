#pragma once

#include "Material.hpp"
#include "Ray.hpp"

#include <memory>
#include <optional>
#include <vector>

struct HitRecord {
    sf::Vector3f point;
    sf::Vector3f normal;
    std::shared_ptr<Material> material;
    float t {};
    bool front_face {};
};

inline void set_face_normal(HitRecord& hit_record, const Ray& ray, const sf::Vector3f& outward_normal)
{
    hit_record.front_face = ray.direction().dot(outward_normal) < 0;
    hit_record.normal = hit_record.front_face ? outward_normal : -outward_normal;
}

class Hittable {
public:
    Hittable() = default;
    virtual ~Hittable() = default;
    Hittable(const Hittable&) = default;
    Hittable& operator=(const Hittable&) = default;
    Hittable(Hittable&&) = default;
    Hittable& operator=(Hittable&&) = default;
    [[nodiscard]] virtual auto hit(const Ray& ray, float t_min, float t_max) const -> std::optional<HitRecord> = 0;
};

[[nodiscard]] auto hit(const std::vector<std::unique_ptr<Hittable>>& world, const Ray& ray, float t_min, float t_max)
    -> std::optional<HitRecord>;
