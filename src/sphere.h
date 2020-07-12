#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere final : public Hittable
{
    point3 center{};
    double radius;
    std::shared_ptr<Material> mat_ptr{};

public:
    Sphere(const point3 cen, const double r, const std::shared_ptr<Material>& m)
        : center(cen)
        , radius(r)
        , mat_ptr(m)
    {}

    virtual bool Hit(const Ray& r,
                     const double t_min,
                     const double t_max,
                     HitRecord& rec) const;
};

bool Sphere::Hit(const Ray& r,
                 const double t_min,
                 const double t_max,
                 HitRecord& rec) const
{
    const auto oc = r.Origin() - center;
    const auto a = r.Direction().Length2();
    const auto half_b = Dot(oc, r.Direction());
    const auto c = oc.Length2() - radius * radius;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const auto root = std::sqrt(discriminant);
        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.At(rec.t);
            const auto outward_normal = (rec.p - center) / radius;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.At(rec.t);
            const auto outward_normal = (rec.p - center) / radius;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}
