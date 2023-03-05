#pragma once

#include "Ray.hpp"

#include <optional>

struct HitRecord {
    sf::Vector3f point;
    sf::Vector3f normal;
    float t {};
};

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
