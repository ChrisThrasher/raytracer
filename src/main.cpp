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

auto RandomScene()
{
    auto world = HittableList();

    const auto ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            const auto choose_mat = RandomDouble();
            const auto center = Point3(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                if (choose_mat < 0.8) // Diffuse
                {
                    const auto albedo = Color::Random() * Color::Random();
                    const auto material = std::make_shared<Lambertian>(albedo);
                    world.Add(std::make_shared<Sphere>(center, 0.2, material));
                }
                else if (choose_mat < 0.95) // Metal
                {
                    const auto albedo = Color::Random(0.5, 1);
                    const auto fuzz = RandomDouble(0, 0.5);
                    const auto material = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, material));
                }
                else // Glass
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

int main()
{
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 120;
    constexpr auto image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 50;
    constexpr auto max_depth = 50;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /*
    auto world = HittableList();
    world.Add(std::make_shared<Sphere>(
        Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5))));
    world.Add(std::make_shared<Sphere>(
        Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0))));
    world.Add(std::make_shared<Sphere>(
        Point3(1, 0, -1), 0.5, std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0)));
    world.Add(std::make_shared<Sphere>(Point3(-1, 0, -1), -0.5, std::make_shared<Dielectric>(1.1)));
    */

    const auto world = RandomScene();

    const auto lookfrom = Point3(13, 2, 3);
    const auto lookat = Point3(0, 0, 0);
    const auto vup = Vec3(0, 1, 0);
    const auto focus_distance = 10;
    const auto aperture = 0.1;
    const auto cam = Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, focus_distance);

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
