#pragma once

#include "Hittable.hpp"

class Sphere final : Hittable {
    sf::Vector3f m_center;
    float m_radius {};
    Material m_material;

public:
    Sphere(const sf::Vector3f& center, float radius, const Material& material) noexcept;

    [[nodiscard]] auto hit(const Ray& ray, float t_min, float t_max) const noexcept -> std::optional<Hit> override;
};
