#include "Material.hpp"
#include "Hittable.hpp"
#include "SFML/System/Vector3.hpp"
#include "Utilities.hpp"

namespace {
auto reflect(const sf::Vector3f& v, const sf::Vector3f& n) { return v - 2.f * v.dot(n) * n; }

auto refract(const sf::Vector3f& uv, const sf::Vector3f& n, const float etai_over_etat)
{
    const auto cos_theta = std::min(-uv.dot(n), 1.f);
    const auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const auto r_out_parallel = -std::sqrt(std::abs(1.f - r_out_perp.lengthSq())) * n;
    return r_out_perp + r_out_parallel;
}
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

Metal::Metal(const sf::Vector3f& color, const float fuzz)
    : m_albedo(color)
    , m_fuzz(std::min(fuzz, 1.f))
{
}

auto Metal::scatter(const Ray& ray, const HitRecord& hit_record) const -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto reflected = reflect(ray.direction().normalized(), hit_record.normal);
    const auto scattered = Ray(hit_record.point, reflected + m_fuzz * random_vector_in_hemisphere(hit_record.normal));
    const auto attenuation = m_albedo;
    if (scattered.direction().dot(hit_record.normal) > 0)
        return std::make_pair(attenuation, scattered);
    return {};
}

Dielectric::Dielectric(const float index_of_refraction)
    : m_index_of_refraction(index_of_refraction)
{
}

auto Dielectric::scatter(const Ray& ray, const HitRecord& hit_record) const
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto refraction_ratio = hit_record.front_face ? 1.f / m_index_of_refraction : m_index_of_refraction;
    const auto unit_direction = ray.direction().normalized();

    const auto cos_theta = std::min(-unit_direction.dot(hit_record.normal), 1.f);
    const auto sin_theta = std::sqrt(1.f - cos_theta * cos_theta);
    const auto cannot_refract = refraction_ratio * sin_theta > 1;

    auto direction = sf::Vector3f();
    if (cannot_refract)
        direction = reflect(unit_direction, hit_record.normal);
    else
        direction = refract(unit_direction, hit_record.normal, refraction_ratio);

    const auto attenuation = sf::Vector3f(1, 1, 1);
    const auto scattered = Ray(hit_record.point, direction);
    return std::make_pair(attenuation, scattered);
}
