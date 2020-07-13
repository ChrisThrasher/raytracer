#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utilities.h"
#include "Vec3.h"

#include <iostream>

auto RayColor(const Ray& r, const Hittable& world, const int depth) -> Color
{
    if (depth <= 0)
        return Color(0, 0, 0);

    auto rec = HitRecord();
    if (world.Hit(r, 0.001, infinity, rec))
    {
        auto scattered = Ray();
        auto attenuation = Color();
        if (rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
            return attenuation * RayColor(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }
    const auto unit_direction = UnitVector(r.Direction());
    const auto t = 0.5 * (unit_direction.Y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 384;
    constexpr auto image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100;
    constexpr auto max_depth = 50;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    auto world = HittableList();
    world.Add(std::make_shared<Sphere>(
        Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5))));
    world.Add(std::make_shared<Sphere>(
        Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0))));
    world.Add(std::make_shared<Sphere>(
        Point3(1, 0, -1), 0.5, std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0)));
    world.Add(std::make_shared<Sphere>(Point3(-1, 0, -1), -0.5, std::make_shared<Dielectric>(1.1)));

    const auto cam = Camera(Point3(-2, 2, 1), Point3(0, 0, -1), Vec3(0, 1, 0), 25, aspect_ratio);

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto pixel_color = Color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + RandomDouble()) / (image_width + 1);
                auto v = (j + RandomDouble()) / (image_height + 1);
                Ray r = cam.GetRay(u, v);
                pixel_color += RayColor(r, world, max_depth);
            }
            WriteColor(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
