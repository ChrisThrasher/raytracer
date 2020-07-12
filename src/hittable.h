#pragma once

#include "ray.h"

struct hit_record
{
    point3 p{};
    vec3 normal{};
    double t{0.0};
};

class hittable
{
public:
    virtual bool his(const ray& r,
                     const double t_min,
                     const double t_max,
                     hit_record& rec) const = 0;
};
