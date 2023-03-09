#pragma once

#include <SFML/System/Vector3.hpp>

#include <random>

auto rng() -> std::mt19937&;
auto random_float(float min, float max) -> float;
auto random_unit_vector() -> sf::Vector3f;
auto random_vector_in_unit_sphere() -> sf::Vector3f;
auto random_vector_in_hemisphere(const sf::Vector3f& normal) -> sf::Vector3f;
auto is_near_zero(const sf::Vector3f& vector) -> bool;