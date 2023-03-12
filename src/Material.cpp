#include "Material.hpp"
#include "Hittable.hpp"
#include "Random.hpp"

namespace {
auto reflect(const sf::Vector3f& v, const sf::Vector3f& n) noexcept { return v - 2.f * v.dot(n) * n; }

auto refract(const sf::Vector3f& uv, const sf::Vector3f& n, const float etai_over_etat) noexcept
{
    const auto cos_theta = std::min(-uv.dot(n), 1.f);
    const auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const auto r_out_parallel = -std::sqrt(std::abs(1.f - r_out_perp.lengthSq())) * n;
    return r_out_perp + r_out_parallel;
}

auto reflectance(const float cosine, const float ref_index) noexcept
{
    // Use Schlick's approximation for reflectance
    const auto r0 = std::pow((1 - ref_index) / (1 + ref_index), 2);
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
}

auto scatter(const Lambertian& lambertian, const Ray& /* ray */, const HitRecord& hit_record) noexcept
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    static constexpr auto epsilon = 1e-9f;
    auto scatter_direction = hit_record.normal + random_unit_vector();
    if (scatter_direction.lengthSq() < epsilon)
        scatter_direction = hit_record.normal;
    return std::make_pair(lambertian.albedo, Ray(hit_record.point, scatter_direction));
}

auto scatter(const Metal& metal, const Ray& ray, const HitRecord& hit_record) noexcept
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto reflected = reflect(ray.direction().normalized(), hit_record.normal);
    const auto scattered
        = Ray(hit_record.point, reflected + metal.fuzz * random_vector_in_hemisphere(hit_record.normal));
    const auto attenuation = metal.albedo;
    if (scattered.direction().dot(hit_record.normal) > 0)
        return std::make_pair(attenuation, scattered);
    return {};
}

auto scatter(const Dielectric& dialectric, const Ray& ray, const HitRecord& hit_record) noexcept
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto refraction_ratio
        = hit_record.front_face ? 1.f / dialectric.index_of_refraction : dialectric.index_of_refraction;
    const auto unit_direction = ray.direction().normalized();

    const auto cos_theta = std::min(-unit_direction.dot(hit_record.normal), 1.f);
    const auto sin_theta = std::sqrt(1.f - cos_theta * cos_theta);
    const auto cannot_refract = refraction_ratio * sin_theta > 1;

    auto direction = sf::Vector3f();
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float(0, 1))
        direction = reflect(unit_direction, hit_record.normal);
    else
        direction = refract(unit_direction, hit_record.normal, refraction_ratio);

    const auto attenuation = sf::Vector3f(1, 1, 1);
    const auto scattered = Ray(hit_record.point, direction);
    return std::make_pair(attenuation, scattered);
}

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

auto scatter(const Material& material, const Ray& ray, const HitRecord& hit_record) noexcept
    -> std::optional<std::pair<sf::Vector3f, Ray>>
{
    const auto overload = Overload {
        [ray, hit_record](const Lambertian& lambertian) noexcept { return scatter(lambertian, ray, hit_record); },
        [ray, hit_record](const Metal& metal) noexcept { return scatter(metal, ray, hit_record); },
        [ray, hit_record](const Dielectric& dialectric) noexcept { return scatter(dialectric, ray, hit_record); },
    };
    return std::visit(overload, material);
}
