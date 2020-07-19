#pragma once

#include "Pixel.h"

template <size_t width>
using Row = std::array<Pixel, width>;
