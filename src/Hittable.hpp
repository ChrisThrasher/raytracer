#pragma once

#include "Material.hpp"
#include "Ray.hpp"

#include <memory>
#include <optional>
#include <vector>

struct HitRecord {
    sf::Vector3f point;
    sf::Vector3f normal;
    std::shared_ptr<const Material> material;
    float t {};
    bool front_face {};
};

void set_face_normal(HitRecord& hit_record, const Ray& ray, const sf::Vector3f& outward_normal) noexcept;

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

using Scene = std::vector<std::unique_ptr<Hittable>>;

[[nodiscard]] auto hit(const Scene& scene, const Ray& ray, float t_min, float t_max) noexcept
    -> std::optional<HitRecord>;
