#pragma once

#include "HittableList.h"
#include "Row.h"

template <size_t width, size_t height>
class Image
{
    std::array<Row<width>, height> m_rows;

public:
    Image()
    {
        size_t row_number = 0;
        for (auto& row : m_rows)
        {
            row = Row<width>(row_number++);
        }
    }

    auto& At(const size_t index) { return m_rows.at(index); }

    friend std::ostream& operator<<(std::ostream& out, const Image<width, height>& image)
    {
        out << "P3\n" << width << ' ' << height << "\n255\n";
        for (auto it = std::cend(image.m_rows) - 1; it >= cbegin(image.m_rows); --it)
        {
            for (const auto& pixel : *it)
            {
                out << pixel;
            }
        }
        return out;
    }
};
