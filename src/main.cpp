#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

bool hit_sphere(const point3& center, const double radius, const ray& r)
{
    const auto oc = r.origin() - center;
    const auto a = dot(r.direction(), r.direction());
    const auto b = 2.0 * dot(oc, r.direction());
    const auto c = dot(oc, oc) - radius * radius;
    const auto discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

auto ray_color(const ray& r)
{
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);
    const auto unit_direction = unit_vector(r.direction());
    const auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const auto image_width = 384;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

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
