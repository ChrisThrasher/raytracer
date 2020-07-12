#pragma once

#include "vec3.h"
#include <iostream>

void WriteColor(std::ostream& out, const color pixel_color, const int samples_per_pixel)
{
    auto r = pixel_color.X();
    auto g = pixel_color.Y();
    auto b = pixel_color.Z();

    // Divide the color total by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    out << static_cast<int>(255.999 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(b, 0.0, 0.999)) << '\n';
}
