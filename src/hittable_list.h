#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
    std::vector<std::shared_ptr<Hittable>> objects;

public:
    HittableList() = default;
    HittableList(const std::shared_ptr<Hittable>& object) { Add(object); }

    void Clear() { objects.clear(); }
    void Add(const std::shared_ptr<Hittable>& object) { objects.push_back(object); }

    virtual bool Hit(const Ray& r,
                     const double t_min,
                     const double t_max,
                     HitRecord& rec) const;
};

bool HittableList::Hit(const Ray& r,
                        const double t_min,
                        const double t_max,
                        HitRecord& rec) const
{
    auto temp_rec = HitRecord();
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
