#pragma once

#include <SFML/System/Vector3.hpp>

#include <random>

[[nodiscard]] auto rng() noexcept -> std::minstd_rand&;
[[nodiscard]] auto random_float(float min, float max) noexcept -> float;
[[nodiscard]] auto random_vector(float min, float max) noexcept -> sf::Vector3f;
[[nodiscard]] auto random_unit_vector() noexcept -> sf::Vector3f;
[[nodiscard]] auto random_vector_in_unit_sphere() noexcept -> sf::Vector3f;
[[nodiscard]] auto random_vector_in_hemisphere(const sf::Vector3f& normal) noexcept -> sf::Vector3f;
[[nodiscard]] auto random_vector_in_unit_disk() noexcept -> sf::Vector3f;
