#pragma once

#include "Camera.h"
#include "World.h"
#include "WriteColor.h"

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

template <size_t image_width, size_t image_height>
void RenderRows(const Camera& camera,
                const World& world,
                const std::vector<Row<image_width>*>& rows)
{
    static std::atomic<size_t> rows_rendered = 0;
    static constexpr auto samples_per_pixel = 50;
    static constexpr auto max_depth = 50;
    for (const auto row : rows)
    {
        for (auto& pixel : *row)
        {
            auto pixel_color = Color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                const auto u = (pixel.u + RandomDouble()) / (image_width + 1);
                const auto v = (pixel.v + RandomDouble()) / (image_height + 1);
                const Ray r = camera.GetRay(u, v);
                pixel_color += RayColor(r, world, max_depth);
            }
            pixel = WriteColor(pixel_color, samples_per_pixel);
        }
        std::cout << "\rScanlines remaining: " << image_height - ++rows_rendered << "    "
                  << std::flush;
    }
};
