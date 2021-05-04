#include <Camera.h>
#include <Image.h>
#include <Render.h>
#include <World.h>

#include <argon/Parser.h>

int main(int argc, char* argv[])
try {
    std::string filename = "image.ppm";

    argon::Parser parser(argc, argv);
    parser.add_option("h,help", "Show this help text", argon::USAGE, "");
    parser.add_option(filename, "o", "Output file name");
    parser.parse();

    constexpr auto aspect_ratio = 16.0 / 9.0;

    const auto camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, aspect_ratio, 0.1, 10);
    const auto world = World::RandomScene();

    constexpr auto image_height = 216ull;
    constexpr auto image_width = static_cast<size_t>(image_height * aspect_ratio);

    RenderImage<image_width, image_height>(camera, world).Write(filename);
} catch (const std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return -1;
}
