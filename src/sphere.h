#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere final : public Hittable
{
    Point3 m_center{};
    double m_radius;
    std::shared_ptr<Material> m_material{};

public:
    Sphere(const Point3 center,
           const double radius,
           const std::shared_ptr<Material>& material)
        : m_center(center)
        , m_radius(radius)
        , m_material(material)
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
    const auto oc = r.Origin() - m_center;
    const auto a = r.Direction().Length2();
    const auto half_b = Dot(oc, r.Direction());
    const auto c = oc.Length2() - m_radius * m_radius;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const auto root = std::sqrt(discriminant);
        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.At(rec.t);
            const auto outward_normal = (rec.p - m_center) / m_radius;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ptr = m_material;
            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.At(rec.t);
            const auto outward_normal = (rec.p - m_center) / m_radius;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ptr = m_material;
            return true;
        }
    }

    return false;
}
