#pragma once

#include "Hittable.h"
#include "Sphere.h"

#include <memory>
#include <vector>

class World : public Hittable {
    std::vector<std::shared_ptr<Hittable>> m_objects;

public:
    World() = default;
    World(const std::shared_ptr<Hittable>& object) { Add(object); }

    void Add(const std::shared_ptr<Hittable>& object) { m_objects.push_back(object); }

    virtual bool Hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const;

    static auto RandomScene()
    {
        auto world = World();

        const auto ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
        world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground));

        for (int a = -11; a < 11; ++a) {
            for (int b = -11; b < 11; ++b) {
                const auto choose_mat = RandomDouble();
                const auto center = Point3(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

                if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                    if (choose_mat < 0.8) // Diffuse
                    {
                        const auto albedo = Color::Random() * Color::Random();
                        const auto material = std::make_shared<Lambertian>(albedo);
                        world.Add(std::make_shared<Sphere>(center, 0.2, material));
                    } else if (choose_mat < 0.95) // Metal
                    {
                        const auto albedo = Color::Random(0.5, 1);
                        const auto fuzz = RandomDouble(0, 0.5);
                        const auto material = std::make_shared<Metal>(albedo, fuzz);
                        world.Add(std::make_shared<Sphere>(center, 0.2, material));
                    } else // Glass
                    {
                        const auto material = std::make_shared<Dielectric>(1.5);
                        world.Add(std::make_shared<Sphere>(center, 0.2, material));
                    }
                }
            }
        }

        const auto material1 = std::make_shared<Dielectric>(1.5);
        world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

        const auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
        world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

        const auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
        world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

        return world;
    }
};

bool World::Hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
    auto temp_rec = HitRecord();
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : m_objects) {
        if (object->Hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
