#pragma once

#include "Ray.hpp"

#include <optional>
#include <utility>
#include <variant>

struct HitRecord;

struct Lambertian {
    sf::Vector3f albedo;
};

struct Metal {
    sf::Vector3f albedo;
    float fuzz {};
};

struct Dielectric {
    float index_of_refraction {};
};

using Material = std::variant<Lambertian, Metal, Dielectric>;

[[nodiscard]] auto scatter(const Material& material, const Ray& ray, const HitRecord& hit_record) noexcept
    -> std::optional<std::pair<sf::Vector3f, Ray>>;
