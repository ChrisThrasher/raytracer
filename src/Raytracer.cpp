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

#include <Options/Parser.h>

int main(int argc, char* argv[])
try
{
    std::string filename = "image.ppm";

    opts::Parser parser(argc, argv, "Usage\n  raytracer [options]");
    parser.Add("o,output_file", "Output file name", opts::Get(filename));
    parser.Parse();

    constexpr auto aspect_ratio = 16.0 / 9.0;

    const auto camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, aspect_ratio, 0.1, 10);
    const auto world = World::RandomScene();

    constexpr auto image_height = 216ull;
    constexpr auto image_width = static_cast<size_t>(image_height * aspect_ratio);

    RenderImage<image_width, image_height>(camera, world).Write(filename);
}
catch (const std::exception& ex)
{
    std::cerr << "Caught unhandled exception: " << ex.what() << '\n';
    return -1;
}
