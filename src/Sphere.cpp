#include "Sphere.hpp"

#include <cmath>

Sphere::Sphere(const sf::Vector3f& center, float radius)
    : m_center(center)
    , m_radius(radius)
{
}

auto Sphere::hit(const Ray& ray, const float t_min, const float t_max) const -> std::optional<HitRecord>
{
    const auto oc = ray.origin() - m_center;
    const auto a = ray.direction().lengthSq();
    const auto half_b = oc.dot(ray.direction());
    const auto c = oc.lengthSq() - m_radius * m_radius;
    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return {};
    const auto sqrt_discriminant = std::sqrtf(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root)
            return {};
    }

    return HitRecord { ray.at(root), ray.at(root), root };
}
