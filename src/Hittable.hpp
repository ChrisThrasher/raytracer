#pragma once

#include "Material.hpp"
#include "Ray.hpp"

#include <optional>

struct Hit {
    sf::Vector3f point;
    sf::Vector3f normal;
    const Material* material {};
    float t {};
    bool front_face {};
};

void set_face_normal(Hit& hit, const Ray& ray, const sf::Vector3f& outward_normal) noexcept;

struct Hittable {
    Hittable() = default;
    virtual ~Hittable() = default;
    Hittable(const Hittable&) = default;
    Hittable& operator=(const Hittable&) = default;
    Hittable(Hittable&&) = default;
    Hittable& operator=(Hittable&&) = default;
    [[nodiscard]] virtual auto hit(const Ray& ray, float t_min, float t_max) const -> std::optional<Hit> = 0;
};
