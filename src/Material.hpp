#pragma once

#include "Ray.hpp"

#include <optional>
#include <utility>

struct HitRecord;

class Material {
public:
    Material() = default;
    virtual ~Material() = default;
    Material(const Material&) = default;
    Material& operator=(const Material&) = default;
    Material(Material&&) = default;
    Material& operator=(Material&&) = default;
    [[nodiscard]] virtual auto scatter(const Ray& ray, const HitRecord& hit_record) const
        -> std::optional<std::pair<sf::Vector3f, Ray>> = 0;
};

class Lambertian : public Material {
    sf::Vector3f m_albedo;

public:
    Lambertian(const sf::Vector3f& color);

    [[nodiscard]] auto scatter(const Ray& ray, const HitRecord& hit_record) const
        -> std::optional<std::pair<sf::Vector3f, Ray>> override;
};

class Metal : public Material {
    sf::Vector3f m_albedo;
    float m_fuzz;

public:
    Metal(const sf::Vector3f& color, float fuzz);

    [[nodiscard]] auto scatter(const Ray& ray, const HitRecord& hit_record) const
        -> std::optional<std::pair<sf::Vector3f, Ray>> override;
};

class Dielectric : public Material {
    float m_index_of_refraction;

public:
    Dielectric(float index_of_refraction);

    [[nodiscard]] auto scatter(const Ray& ray, const HitRecord& hit_record) const
        -> std::optional<std::pair<sf::Vector3f, Ray>> override;
};
