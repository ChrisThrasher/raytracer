#include "rtweekend.h"

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

constexpr auto ray_color(const ray& r, const hittable& world, const int depth)
    -> color
{
    if (depth <= 0)
        return color(0, 0, 0);

    auto rec = hit_record();
    if (world.hit(r, 0, infinity, rec))
    {
        auto target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }
    const auto unit_direction = unit_vector(r.direction());
    const auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 384;
    constexpr auto image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100;
    constexpr auto max_depth = 50;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    auto world = hittable_list();
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    constexpr auto cam = camera();

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto pixel_color = color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width + 1);
                auto v = (j + random_double()) / (image_height + 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
