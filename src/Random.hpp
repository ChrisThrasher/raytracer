#pragma once

#include <random>

auto rng() -> std::mt19937&;
auto random_double(float min, float max) -> float;
