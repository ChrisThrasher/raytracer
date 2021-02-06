#pragma once

#include "Pixel.h"

#include <array>
#include <cstddef>

template <size_t width>
class Row : public std::array<Pixel, width> {
public:
    Row() = default;
    Row(const uint16_t row_number)
    {
        uint16_t col_number = 0;
        for (auto& pixel : *this) {
            pixel.u = col_number++;
            pixel.v = row_number;
        }
    }
};
