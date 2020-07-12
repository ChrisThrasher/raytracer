#pragma once

#include "ray.h"

class Material;

struct HitRecord
{
    point3 p{};
    Vec3 normal{};
    std::shared_ptr<Material> mat_ptr{};
    double t{0.0};
    bool front_face{false};

    inline void SetFaceNormal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = Dot(r.Direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool Hit(const Ray& r,
                     const double t_min,
                     const double t_max,
                     HitRecord& rec) const = 0;
};
