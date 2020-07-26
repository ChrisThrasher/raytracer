#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utilities.h"
#include "Vec3.h"
#include "World.h"
#include "WriteColor.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

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
    auto world = World();

    /*
    world.Add(std::make_shared<Sphere>(
        Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5))));
    world.Add(std::make_shared<Sphere>(
        Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0))));
    world.Add(std::make_shared<Sphere>(
        Point3(1, 0, -1), 0.5, std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0)));
    world.Add(std::make_shared<Sphere>(Point3(-1, 0, -1), -0.5, std::make_shared<Dielectric>(1.1)));
    */

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

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Must specify filename.\n";
        return -1;
    }

    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_height = 216ull;
    constexpr auto image_width = static_cast<size_t>(image_height * aspect_ratio);
    constexpr auto samples_per_pixel = 50;
    constexpr auto max_depth = 50;
    constexpr auto num_threads = 12;
    constexpr auto rows_per_thread = image_height / num_threads;
    static_assert(image_height % num_threads == 0, "");
    static_assert(num_threads <= image_height, "");

    constexpr auto lookfrom = Point3(13, 2, 3);
    constexpr auto lookat = Point3(0, 0, 0);
    constexpr auto vup = Vec3(0, 1, 0);
    constexpr auto focus_distance = 10;
    constexpr auto aperture = 0.1;
    const auto cam = Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, focus_distance);

    const auto world = RandomScene();

    const auto render_rows = [cam, world](const std::vector<Row<image_width>*>& rows) {
        static std::atomic<size_t> rows_rendered = 0;
        for (const auto row : rows)
        {
            for (auto& pixel : *row)
            {
                auto pixel_color = Color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s)
                {
                    const auto u = (pixel.u + RandomDouble()) / (image_width + 1);
                    const auto v = (pixel.v + RandomDouble()) / (image_height + 1);
                    const Ray r = cam.GetRay(u, v);
                    pixel_color += RayColor(r, world, max_depth);
                }
                pixel = WriteColor(pixel_color, samples_per_pixel);
            }
            std::cout << "\rScanlines remaining: " << image_height - ++rows_rendered << "    "
                      << std::flush;
        }
    };

    auto image = Image<image_width, image_height>();
    auto threads = std::array<std::thread, num_threads>();
    const auto start_time = std::chrono::system_clock::now();
    for (size_t i = 0; i < threads.size(); ++i)
    {
        std::vector<Row<image_width>*> rows;
        for (size_t j = 0; j < rows_per_thread; ++j)
        {
            rows.push_back(&image.At(i * rows_per_thread + j));
        }
        threads.at(i) = std::thread(render_rows, rows);
    }

    const auto filename = argv[1];
    std::cout << "Writing " << image_height << "x" << image_width << " image to " << filename
              << ".\n";
    std::cout << "Spawned " << threads.size() << " thread(s).\n";
    std::cout << "Hardware supports " << std::thread::hardware_concurrency() << " threads.\n";
    std::cout << "Rendering " << rows_per_thread << " rows per thread.\n";
    std::cout << "Scanlines remaining: " << image_height << std::flush;

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "\rFinished rendering in "
              << (std::chrono::system_clock::now() - start_time).count() / 1'000'000.0
              << " seconds.\n";

    std::ofstream output_file(filename);
    output_file << image;
    std::cout << "Done. Total runtime = "
              << (std::chrono::system_clock::now() - start_time).count() / 1'000'000.0
              << " seconds.\n";
}
