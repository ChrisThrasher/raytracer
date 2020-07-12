#pragma once

#include <cmath>
#include <array>
#include <iostream>

class Vec3
{
    std::array<double, 3> e{};

public:
    constexpr Vec3() = default;
    constexpr Vec3(const double e1, const double e2, const double e3)
        : e{e1, e2, e3}
    {}

    constexpr auto X() const { return e[0]; }
    constexpr auto Y() const { return e[1]; }
    constexpr auto Z() const { return e[2]; }

    constexpr auto operator~() const { return Vec3(-e[0], -e[1], -e[2]); }
    constexpr auto operator[](size_t i) const { return e.at(i); }
    constexpr auto& operator[](size_t i) { return e.at(i); }

    constexpr auto& operator+=(const Vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    constexpr auto& operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    constexpr auto& operator/=(const double t)
    {
        return *this *= 1/t;
    }

    constexpr auto Length2() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    auto Length() const
    {
        return std::sqrt(Length2());
    }

    inline static auto Random()
    {
        return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
    }

    inline static auto Random(const double min, const double max)
    {
        return Vec3(RandomDouble(min, max),
                    RandomDouble(min, max),
                    RandomDouble(min, max));
    }
};

using point3 = Vec3;
using color = Vec3;

inline auto& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.X() << ' ' << v.Y() << ' ' << v.Z();
}

constexpr inline auto operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() + rhs.X(), lhs.Y() + rhs.Y(), lhs.Z() + rhs.Z());
}

constexpr inline auto operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() - rhs.X(), lhs.Y() - rhs.Y(), lhs.Z() - rhs.Z());
}

constexpr inline auto operator-(const Vec3& v)
{
    return Vec3(-v.X(), -v.Y(), -v.Z());
}

constexpr inline auto operator*(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.X() * rhs.X(), lhs.Y() * rhs.Y(), lhs.Z() * rhs.Z());
}

constexpr inline auto operator*(const double t, const Vec3& v)
{
    return Vec3(t * v.X(), t * v.Y(), t * v.Z());
}

constexpr inline auto operator*(const Vec3& v, const double t)
{
    return t * v;
}

constexpr inline auto operator/(const Vec3& v, const double t)
{
    return (1/t) * v;
}

constexpr inline auto Dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.X() * rhs.X()
         + lhs.Y() * rhs.Y()
         + lhs.Z() * rhs.Z();
}

constexpr inline auto Cross(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
                lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
                lhs.X() * rhs.Y() - lhs.Y() * rhs.X());
}

inline auto UnitVector(const Vec3& v)
{
    return v / v.Length();
}

auto RandomInUnitSphere()
{
    while (true)
    {
        const auto p = Vec3::Random(-1, 1);
        if (p.Length2() >= 1) continue;
        return p;
    }
}

auto RandomUnitVector()
{
    auto a = RandomDouble(0, 2 * pi);
    auto z = RandomDouble(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return Vec3(r * std::cos(a), r * std::sin(a), z);
}

auto RandomInHemisphere(const Vec3& normal)
{
    auto in_unit_sphere = RandomInUnitSphere();
    if (Dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

auto Reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * Dot(v, n) * n;
}

auto Refract(const Vec3& uv, const Vec3& n, const double etai_over_etat)
{
    const auto cos_theta = Dot(-uv, n);
    auto r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    auto r_out_perp = -std::sqrt(1.0 - r_out_parallel.Length2()) * n;
    return r_out_parallel + r_out_perp;
}
