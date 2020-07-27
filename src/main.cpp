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

#include <iostream>

int main(int argc, char* argv[]) try
{
    if (argc < 2)
    {
        std::cerr << "Must specify filename.\n";
        return -1;
    }
    const auto filename = argv[1];

    constexpr auto aspect_ratio = 16.0 / 9.0;

    const auto camera = []() {
        constexpr auto lookfrom = Point3(13, 2, 3);
        constexpr auto lookat = Point3(0, 0, 0);
        constexpr auto vup = Vec3(0, 1, 0);
        constexpr auto focus_distance = 10;
        constexpr auto aperture = 0.1;
        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, focus_distance);
    }();

    const auto world = World::RandomScene();

    constexpr auto image_height = 216ull;
    constexpr auto image_width = static_cast<size_t>(image_height * aspect_ratio);
    const auto image = RenderImage<image_width, image_height>(camera, world);

    image.Write(filename);
}
catch (...)
{
    std::cerr << "\nCaught unhandled exception";
}
