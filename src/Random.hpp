#pragma once

#include <SFML/System/Vector3.hpp>

#include <random>

auto rng() -> std::mt19937&;
auto random_float() -> float;
auto random_float(float min, float max) -> float;
auto random_vector(float min, float max) -> sf::Vector3f;
auto random_unit_vector() -> sf::Vector3f;
