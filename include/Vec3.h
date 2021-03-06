#pragma once

#include "Utilities.h"

#include <array>
#include <cmath>
#include <iostream>

class Vec3 {
    std::array<double, 3> m_e {};

public:
    constexpr Vec3() = default;
    constexpr Vec3(const double e1, const double e2, const double e3)
        : m_e { e1, e2, e3 }
    {
    }

    constexpr auto X() const { return m_e[0]; }
    constexpr auto Y() const { return m_e[1]; }
    constexpr auto Z() const { return m_e[2]; }

    constexpr auto operator~() const { return Vec3(-m_e[0], -m_e[1], -m_e[2]); }
    constexpr auto operator[](size_t i) const { return m_e.at(i); }
    constexpr auto& operator[](size_t i) { return m_e.at(i); }

    constexpr auto& operator+=(const Vec3& v)
    {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];
        return *this;
    }

    constexpr auto& operator*=(const double t)
    {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;
        return *this;
    }

    constexpr auto& operator/=(const double t) { return *this *= 1 / t; }

    constexpr auto Length2() const { return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]; }

    auto Length() const { return std::sqrt(Length2()); }

    inline static auto Random() { return Vec3(RandomDouble(), RandomDouble(), RandomDouble()); }

    inline static auto Random(const double min, const double max)
    {
        return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }
};

using Point3 = Vec3;
using Color = Vec3;

inline auto& operator<<(std::ostream& out, const Vec3& v) { return out << v.X() << ' ' << v.Y() << ' ' << v.Z(); }

constexpr inline auto operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() + rhs.X(), lhs.Y() + rhs.Y(), lhs.Z() + rhs.Z());
}

constexpr inline auto operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() - rhs.X(), lhs.Y() - rhs.Y(), lhs.Z() - rhs.Z());
}

constexpr inline auto operator-(const Vec3& v) { return Vec3(-v.X(), -v.Y(), -v.Z()); }

constexpr inline auto operator*(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() * rhs.X(), lhs.Y() * rhs.Y(), lhs.Z() * rhs.Z());
}

constexpr inline auto operator*(const double t, const Vec3& v) { return Vec3(t * v.X(), t * v.Y(), t * v.Z()); }

constexpr inline auto operator*(const Vec3& v, const double t) { return t * v; }

constexpr inline auto operator/(const Vec3& v, const double t) { return (1 / t) * v; }

constexpr inline auto Dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.X() * rhs.X() + lhs.Y() * rhs.Y() + lhs.Z() * rhs.Z();
}

constexpr inline auto Cross(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
                lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
                lhs.X() * rhs.Y() - lhs.Y() * rhs.X());
}

inline auto UnitVector(const Vec3& v) { return v / v.Length(); }

auto Reflect(const Vec3& v, const Vec3& n) { return v - 2 * Dot(v, n) * n; }

auto Refract(const Vec3& uv, const Vec3& n, const double etai_over_etat)
{
    const auto cos_theta = Dot(-uv, n);
    auto r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    auto r_out_perp = -std::sqrt(1.0 - r_out_parallel.Length2()) * n;
    return r_out_parallel + r_out_perp;
}
