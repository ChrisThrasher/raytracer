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
