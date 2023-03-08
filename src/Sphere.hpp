#pragma once

#include "Hittable.hpp"

class Sphere : public Hittable {
    sf::Vector3f m_center;
    float m_radius {};
    std::shared_ptr<Material> m_material;

public:
    Sphere(const sf::Vector3f& center, float radius, std::shared_ptr<Material> material);

    [[nodiscard]] auto hit(const Ray& ray, float t_min, float t_max) const -> std::optional<HitRecord> override;
};
