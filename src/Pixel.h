#pragma once

struct Pixel
{
    uint8_t r{0};
    uint8_t g{255};
    uint8_t b{0};
    uint16_t u{0};
    uint16_t v{0};
};

void operator<<(std::ostream& ostream, const Pixel& pixel)
{
    ostream << static_cast<int>(pixel.r) << ' ' << static_cast<int>(pixel.g) << ' '
            << static_cast<int>(pixel.b) << '\n';
}
