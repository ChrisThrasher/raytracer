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
