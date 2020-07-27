#pragma once

#include "Pixel.h"
#include "Vec3.h"

#include <iostream>

Pixel WriteColor(const Color pixel_color, const int samples_per_pixel)
{
    const auto r = std::sqrt(pixel_color.X() / samples_per_pixel);
    const auto g = std::sqrt(pixel_color.Y() / samples_per_pixel);
    const auto b = std::sqrt(pixel_color.Z() / samples_per_pixel);

    return Pixel{static_cast<uint8_t>(255.999 * std::clamp(r, 0.0, 0.999)),
                 static_cast<uint8_t>(255.999 * std::clamp(g, 0.0, 0.999)),
                 static_cast<uint8_t>(255.999 * std::clamp(b, 0.0, 0.999))};
}
