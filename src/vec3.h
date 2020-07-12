#pragma once

#include <cmath>
#include <array>
#include <iostream>

class vec3
{
    std::array<double, 3> e{};

public:
    constexpr vec3() = default;
    constexpr vec3(const double e1, const double e2, const double e3)
        : e{e1, e2, e3}
    {}

    constexpr auto x() const { return e[0]; }
    constexpr auto y() const { return e[1]; }
    constexpr auto z() const { return e[2]; }

    constexpr auto operator~() const { return vec3(-e[0], -e[1], -e[2]); }
    constexpr auto operator[](size_t i) const { return e.at(i); }
    constexpr auto& operator[](size_t i) { return e.at(i); }

    constexpr auto& operator+=(const vec3& v)
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

    constexpr auto length2() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    auto length() const
    {
        return std::sqrt(length2());
    }

    inline static auto random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static auto random(const double min, const double max)
    {
        return vec3(random_double(min, max),
                    random_double(min, max),
                    random_double(min, max));
    }
};

using point3 = vec3;
using color = vec3;

inline auto& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

constexpr inline auto operator+(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

constexpr inline auto operator-(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

constexpr inline auto operator-(const vec3& v)
{
    return vec3(-v.x(), -v.y(), -v.z());
}

constexpr inline auto operator*(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

constexpr inline auto operator*(const double t, const vec3& v)
{
    return vec3(t * v.x(), t * v.y(), t * v.z());
}

constexpr inline auto operator*(const vec3& v, const double t)
{
    return t * v;
}

constexpr inline auto operator/(const vec3& v, const double t)
{
    return (1/t) * v;
}

constexpr inline auto dot(const vec3& lhs, const vec3& rhs)
{
    return lhs.x() * rhs.x()
         + lhs.y() * rhs.y()
         + lhs.z() * rhs.z();
}

constexpr inline auto cross(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                lhs.x() * rhs.y() - lhs.y() * rhs.x());
}

inline auto unit_vector(const vec3& v)
{
    return v / v.length();
}

auto random_in_unit_sphere()
{
    while (true)
    {
        const auto p = vec3::random(-1, 1);
        if (p.length2() >= 1) continue;
        return p;
    }
}

auto random_unit_vector()
{
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return vec3(r * std::cos(a), r * std::sin(a), z);
}

auto random_in_hemisphere(const vec3& normal)
{
    auto in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

auto reflect(const vec3& v, const vec3& n)
{
    reeturn v - 2 * dot(v, n) * n;
}
