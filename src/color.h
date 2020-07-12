#pragma once

#include "vec3.h"
#include <iostream>

void write_color(std::ostream& out, const color pixel_color, const int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color total by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    out << static_cast<int>(255.999 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(b, 0.0, 0.999)) << '\n';
}
