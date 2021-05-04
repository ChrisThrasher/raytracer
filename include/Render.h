#pragma once

#include "Camera.h"
#include "World.h"
#include "WriteColor.h"

#include <atomic>
#include <chrono>
#include <iomanip>
#include <optional>
#include <thread>

template <size_t image_width, size_t image_height>
class RenderQueue {
    Image<image_width, image_height>* const m_image;
    std::atomic<uint16_t> m_allocated_rows { 0 };

public:
    RenderQueue(Image<image_width, image_height>* const image)
        : m_image(image)
    {
    }

    Row<image_width>* Pop()
    {
        if (m_allocated_rows < image_height) {
            return &m_image->At(m_allocated_rows++);
        } else {
            return nullptr;
        }
    }
};

auto RayColor(const Ray& r, const Hittable& world, const int depth) -> Color
{
    if (depth <= 0)
        return Color(0, 0, 0);

    auto rec = HitRecord();
    if (world.Hit(r, 0.001, infinity, rec)) {
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
void RenderRows(const Camera& camera, const World& world, RenderQueue<image_width, image_height>* queue)
{
    static std::atomic<size_t> rows_rendered = 0;
    static constexpr auto samples_per_pixel = 15;
    static constexpr auto max_depth = 5;
    auto* row = queue->Pop();
    while (row != nullptr) {
        for (auto& pixel : *row) {
            auto pixel_color = Color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                const auto u = (static_cast<double>(pixel.u) + RandomDouble()) / (image_width + 1);
                const auto v = (static_cast<double>(pixel.v) + RandomDouble()) / (image_height + 1);
                const Ray r = camera.GetRay(u, v);
                pixel_color += RayColor(r, world, max_depth);
            }
            pixel = WriteColor(pixel_color, samples_per_pixel);
        }
        std::cout << "\rScanlines remaining: " << std::setfill(' ') << std::right << std::setw(4)
                  << image_height - ++rows_rendered << std::flush;
        row = queue->Pop();
    }
}

template <size_t image_width, size_t image_height>
auto RenderImage(const Camera& camera, const World& world) -> Image<image_width, image_height>
{
    static constexpr auto num_threads = 8;
    static_assert(num_threads <= image_height, "");

    auto image = Image<image_width, image_height>();
    auto threads = std::array<std::thread, num_threads>();
    auto queue = RenderQueue<image_width, image_height>(&image);
    const auto start_time = std::chrono::system_clock::now();
    for (auto& thread : threads)
        thread = std::thread(RenderRows<image_width, image_height>, camera, world, &queue);

    std::cout << "Spawned " << threads.size() << " threads. (Hardware supports " << std::thread::hardware_concurrency()
              << " concurrent threads)\n";
    std::cout << "Rendering " << image_height << "x" << image_width << " image...\n";
    std::cout << "\rScanlines remaining: " << std::setfill(' ') << std::right << std::setw(4) << image_height;

    for (auto& thread : threads) {
        thread.join();
    }

    const auto runtime = static_cast<double>((std::chrono::system_clock::now() - start_time).count()) / 1'000'000.0;
    std::cout << "\rFinished rendering in " << runtime << " seconds. ("
              << static_cast<int>(image_width * image_height / runtime) << " pixels per second)\n";

    return image;
}
