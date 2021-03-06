#pragma once

#include <cstdint>
#include <ostream>

struct Pixel {
    uint8_t r { 0 };
    uint8_t g { 255 };
    uint8_t b { 0 };
    uint16_t u { 0 };
    uint16_t v { 0 };
};

void operator<<(std::ostream& ostream, const Pixel& pixel)
{
    ostream << +pixel.r << ' ' << +pixel.g << ' ' << +pixel.b << '\n';
}
