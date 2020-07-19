#pragma once

#include "HittableList.h"
#include "Row.h"

template <size_t width, size_t height>
class Image : public std::array<Row<width>, height>
{
    friend std::ostream& operator<<(std::ostream& out, const Image<width, height>& image)
    {
        out << "P3\n" << width << ' ' << height << "\n255\n";
        for (auto it = std::cend(image) - 1; it >= cbegin(image); --it)
        {
            for (const auto& pixel : *it)
            {
                out << pixel;
            }
        }
        return out;
    }
};
