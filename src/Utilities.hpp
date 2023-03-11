#pragma once

#include <SFML/System/Vector3.hpp>

#include <random>

auto rng() noexcept -> std::minstd_rand&;
auto random_float(float min, float max) noexcept -> float;
auto random_vector(float min, float max) noexcept -> sf::Vector3f;
auto random_unit_vector() noexcept -> sf::Vector3f;
auto random_vector_in_unit_sphere() noexcept -> sf::Vector3f;
auto random_vector_in_hemisphere(const sf::Vector3f& normal) noexcept -> sf::Vector3f;
auto random_vector_in_unit_disk() noexcept -> sf::Vector3f;
auto is_near_zero(const sf::Vector3f& vector) noexcept -> bool;
