#pragma once

#include "Pixel.h"

template <size_t width>
class Row : public std::array<Pixel, width>
{
public:
    Row() = default;
    Row(const size_t row_number)
    {
        size_t col_number = 0;
        for (auto& pixel : *this)
        {
            pixel.u = col_number++;
            pixel.v = row_number;
        }
    }
};
