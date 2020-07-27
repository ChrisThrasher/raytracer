#pragma once

#include "Row.h"

#include <fstream>

template <size_t width, size_t height>
class Image
{
    std::array<Row<width>, height> m_rows;

public:
    Image()
    {
        uint16_t row_number = 0;
        for (auto& row : m_rows)
        {
            row = Row<width>(row_number++);
        }
    }

    auto& At(const size_t index) { return m_rows.at(index); }

    void Write(const std::string& filename) const
    {
        std::ofstream output_file(filename);
        output_file << "P3\n" << width << ' ' << height << "\n255\n";
        for (auto it = std::cend(m_rows) - 1; it >= cbegin(m_rows); --it)
        {
            for (const auto& pixel : *it)
            {
                output_file << pixel;
            }
        }
        std::cout << "Wrote image to " << filename << ".\n";
    }
};
