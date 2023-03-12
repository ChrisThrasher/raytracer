#pragma once

#include "Hittable.hpp"

class Sphere : public Hittable {
    sf::Vector3f m_center;
    float m_radius {};
    std::unique_ptr<const Material> m_material;

public:
    Sphere(const sf::Vector3f& center, float radius, std::unique_ptr<const Material> material) noexcept;

    [[nodiscard]] auto hit(const Ray& ray, float t_min, float t_max) const noexcept
        -> std::optional<HitRecord> override;
};
