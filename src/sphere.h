#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere final : public hittable
{
    point3 center{};
    double radius;
    std::shared_ptr<material> mat_ptr{};

public:
    sphere(const point3 cen, const double r, const std::shared_ptr<material> m)
        : center(cen)
        , radius(r)
        , mat_ptr(m)
    {}

    virtual bool hit(const ray& r,
                     const double t_min,
                     const double t_max,
                     hit_record& rec) const;
};

bool sphere::hit(const ray& r,
                 const double t_min,
                 const double t_max,
                 hit_record& rec) const
{
    const auto oc = r.origin() - center;
    const auto a = r.direction().length2();
    const auto half_b = dot(oc, r.direction());
    const auto c = oc.length2() - radius * radius;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const auto root = std::sqrt(discriminant);
        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);
            const auto outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);
            const auto outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}
