#include <iostream>

int main()
{
    const auto image_width = 256;
    const auto image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            const auto r = double(i) / (image_width - 1);
            const auto g = double(j) / (image_height - 1);
            const auto b = 0.25;

            const auto ir = static_cast<int>(255.999 * r);
            const auto ig = static_cast<int>(255.999 * g);
            const auto ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    return 0;
}
