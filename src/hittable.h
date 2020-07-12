#pragma once

#include "ray.h"

struct hit_record
{
    point3 p{};
    vec3 normal{};
    double t{0.0};
    bool front_face{false};

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray& r,
                     const double t_min,
                     const double t_max,
                     hit_record& rec) const = 0;
};
