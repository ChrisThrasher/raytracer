#pragma once

#include "Vec3.h"

#include <iostream>

void WriteColor(std::ostream& out, const Color pixel_color, const int samples_per_pixel)
{
    const auto scale = 1.0 / samples_per_pixel;

    const auto r = std::sqrt(scale * pixel_color.X());
    const auto g = std::sqrt(scale * pixel_color.Y());
    const auto b = std::sqrt(scale * pixel_color.Z());

    out << static_cast<int>(255.999 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * std::clamp(b, 0.0, 0.999)) << '\n';
}
