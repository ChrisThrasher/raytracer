#include "Material.hpp"
#include "Hittable.hpp"
#include "Utilities.hpp"

namespace {
auto reflect(const sf::Vector3f& v, const sf::Vector3f& n) { return v - 2.f * v.dot(n) * n; }
}

Lambertian::Lambertian(const sf::Vector3f& color)
    : m_albedo(color)
{
}

auto Lambertian::scatter(const Ray& /* ray */, const HitRecord& hit_record) const
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    auto scatter_direction = hit_record.normal + random_unit_vector();
    if (is_near_zero(scatter_direction))
        scatter_direction = hit_record.normal;
    return std::make_pair(m_albedo, Ray(hit_record.point, scatter_direction));
}

Metal::Metal(const sf::Vector3f& color)
    : m_albedo(color)
{
}

auto Metal::scatter(const Ray& ray, const HitRecord& hit_record) const -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto reflected = reflect(ray.direction().normalized(), hit_record.normal);
    const auto scattered = Ray(hit_record.point, reflected);
    const auto attenuation = m_albedo;
    if (scattered.direction().dot(hit_record.normal) > 0)
        return std::make_pair(attenuation, scattered);
    return {};
}
