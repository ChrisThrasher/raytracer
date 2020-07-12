#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

auto ray_color(const ray& r)
{
    const auto unit_direction = unit_vector(r.direction());
    const auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    const auto image_width = 256;
    const auto image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    const auto aspect_ratio = 16.0 / 9.0;
    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height;
    const auto focal_length = 1.0;

    const auto origin = point3();
    const auto horizontal = vec3(viewport_width, 0, 0);
    const auto vertical = vec3(0, viewport_height, 0);
    const auto lower_left_corner = origin
                                 - horizontal / 2
                                 - vertical / 2
                                 - vec3(0, 0, focal_length);

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            const auto u = double(i) / (image_width - 1);
            const auto v = double(j) / (image_height - 1);
            const auto r = ray(origin,
                               lower_left_corner
                               + u * horizontal
                               + v * vertical
                               - origin);
            const auto pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}
