#pragma once

#include "ray.h"
#include "hittable.h"

class material
{
public:
    virtual bool scatter(const ray& r_in,
                         const hit_record& rec,
                         color& attenuation,
                         ray& scattered) const = 0;
};

class lambertian final : public material
{
    color albedo{};

public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray&,
                         const hit_record& rec,
                         color& attenuation,
                         ray& scattered) const
    {
        auto scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class metal final : public material
{
    color albedo{};
    double fuzz{0.0};

public:
    metal(const color& a, const double f)
        : albedo(a)
        , fuzz(f < 1 ? f : 1)
    {}

    virtual bool scatter(const ray& r_in,
                         const hit_record& rec,
                         color& attenuation,
                         ray& scattered) const
    {
        auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};

class dielectric final : public material
{
    double ref_idx{0.0};

public:
    dielectric(const double ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r_in,
                         const hit_record& rec,
                         color& attenuation,
                         ray& scattered) const
    {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = 0;
        if (rec.front_face) // TODO use ternary here
        {
            etai_over_etat = 1.0 / ref_idx;
        }
        else
        {
            etai_over_etat = ref_idx;
        }

        auto unit_direction = unit_vector(r_in.direction());
        auto refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
};
