#include "vec3.h"
#include "color.h"

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
            const auto pixel_color = color(double(i) / (image_width - 1),
                                           double(j) / (image_height - 1),
                                           0.25);
            write_color(std::cout, pixel_color);
        }
    }

    return 0;
}
