#pragma once

struct Pixel
{
    int r{0};
    int g{255};
    int b{0};
};

void operator<<(std::ostream& ostream, const Pixel& pixel)
{
    ostream << pixel.r << ' ' << pixel.g << ' ' << pixel.b << '\n';
}
