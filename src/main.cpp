#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Ray.h"
#include "Render.h"
#include "Sphere.h"
#include "Utilities.h"
#include "Vec3.h"
#include "World.h"
#include "WriteColor.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

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
    constexpr auto num_threads = 8;
    constexpr auto rows_per_thread = image_height / num_threads;
    static_assert(image_height % num_threads == 0, "");
    static_assert(num_threads <= image_height, "");

    const auto camera = []() {
        constexpr auto lookfrom = Point3(13, 2, 3);
        constexpr auto lookat = Point3(0, 0, 0);
        constexpr auto vup = Vec3(0, 1, 0);
        constexpr auto focus_distance = 10;
        constexpr auto aperture = 0.1;
        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, focus_distance);
    }();

    const auto world = World::RandomScene();

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
        threads.at(i) = std::thread(RenderRows<image_width, image_height>, camera, world, rows);
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
