#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
    std::vector<std::shared_ptr<hittable>> objects;

public:
    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r,
                     const double t_min,
                     const double t_max,
                     hit_record& rec) const;
};

bool hittable_list::hit(const ray& r,
                        const double t_min,
                        const double t_max,
                        hit_record& rec) const
{
    auto temp_rec = hit_record();
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
