#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

auto hit_sphere(const point3& center, const double radius, const ray& r)
{
    const auto oc = r.origin() - center;
    const auto a = r.direction().length2();
    const auto half_b = dot(oc, r.direction());
    const auto c = oc.length2() - radius * radius;
    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - std::sqrt(discriminant)) / a;
    }
}

auto ray_color(const ray& r)
{
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        const auto N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    const auto unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
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
