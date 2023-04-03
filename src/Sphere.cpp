#include "Sphere.hpp"

#include <cmath>

Sphere::Sphere(const sf::Vector3f& center, const float radius, const Material& material) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_material(material)
{
}

auto Sphere::hit(const Ray& ray, const float t_min, const float t_max) const noexcept -> std::optional<Hit>
{
    const auto oc = ray.origin() - m_center;
    const auto a = ray.direction().lengthSq();
    const auto half_b = oc.dot(ray.direction());
    const auto c = oc.lengthSq() - m_radius * m_radius;
    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return {};
    const auto sqrt_discriminant = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root)
            return {};
    }

    Hit hit;
    hit.t = root;
    hit.point = ray.at(root);
    const auto outward_normal = (hit.point - m_center) / m_radius;
    set_face_normal(hit, ray, outward_normal);
    hit.material = &m_material;
    return hit;
}
