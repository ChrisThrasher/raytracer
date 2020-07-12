#pragma once

#include "ray.h"
#include "hittable.h"

#include <cmath>

auto Schlick(const double cosine, const double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

class Material
{
public:
    virtual bool Scatter(const Ray& r_in,
                         const HitRecord& rec,
                         color& attenuation,
                         Ray& scattered) const = 0;
};

class Lambertian final : public Material
{
    color m_albedo{};

public:
    Lambertian(const color& a) : m_albedo(a) {}

    virtual bool Scatter(const Ray&,
                         const HitRecord& rec,
                         color& attenuation,
                         Ray& scattered) const
    {
        auto scatter_direction = rec.normal + RandomUnitVector();
        scattered = Ray(rec.p, scatter_direction);
        attenuation = m_albedo;
        return true;
    }
};

class Metal final : public Material
{
    color m_albedo{};
    double m_fuzz{0.0};

public:
    Metal(const color& a, const double f)
        : m_albedo(a)
        , m_fuzz(f < 1 ? f : 1)
    {}

    virtual bool Scatter(const Ray& r_in,
                         const HitRecord& rec,
                         color& attenuation,
                         Ray& scattered) const
    {
        auto reflected = Reflect(UnitVector(r_in.Direction()), rec.normal);
        scattered = Ray(rec.p, reflected + m_fuzz * RandomInUnitSphere());
        attenuation = m_albedo;
        return Dot(scattered.Direction(), rec.normal) > 0;
    }
};

class Dielectric final : public Material
{
    double m_ref_idx{0.0};

public:
    Dielectric(const double ri) : m_ref_idx(ri) {}

    virtual bool Scatter(const Ray& r_in,
                         const HitRecord& rec,
                         color& attenuation,
                         Ray& scattered) const
    {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / m_ref_idx) : m_ref_idx;
        auto unit_direction = UnitVector(r_in.Direction());
        double cos_theta = std::fmin(Dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0)
        {
            auto reflected = Reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = Schlick(cos_theta, etai_over_etat);
        if (RandomDouble() < reflect_prob)
        {
            auto reflected = Reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected);
            return true;
        }
        auto refracted = Refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray(rec.p, refracted);
        return true;
    }
};
