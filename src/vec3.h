#pragma once

#include <cmath>
#include <array>
#include <iostream>

class vec3
{
public:
    std::array<double, 3> e{};

    vec3(const double e1, const double e2, const double e3)
        : e{e1, e2, e3}
    {}

    auto x() const { return e[0]; }
    auto y() const { return e[1]; }
    auto z() const { return e[2]; }

    auto operator~() const { return vec3(-e[0], -e[1], -e[2]); }
    auto operator[](size_t i) const { return e.at(i); }
    auto& operator[](size_t i) { return e.at(i); }

    auto& operator+=(const vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    auto& operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    auto& operator/=(const double t)
    {
        return *this *= 1/t;
    }

    auto length2() const
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
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline auto operator+(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.e[0] + rhs.e[0], lhs.e[1] + rhs.e[1], lhs.e[2] + rhs.e[2]);
}

inline auto operator-(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.e[0] - rhs.e[0], lhs.e[1] - rhs.e[1], lhs.e[2] - rhs.e[2]);
}

inline auto operator*(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.e[0] * rhs.e[0], lhs.e[1] * rhs.e[1], lhs.e[2] * rhs.e[2]);
}

inline auto operator*(const double t, const vec3& v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline auto operator*(const vec3& v, const double t)
{
    return t * v;
}

inline auto operator/(const vec3& v, const double t)
{
    return (1/t) * v;
}

inline auto dot(const vec3& lhs, const vec3& rhs)
{
    return lhs.e[0] * rhs.e[0]
         + lhs.e[1] * rhs.e[1]
         + lhs.e[2] * rhs.e[2];
}

inline auto cross(const vec3& lhs, const vec3& rhs)
{
    return vec3(lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1],
                lhs.e[2] * rhs.e[0] - lhs.e[0] * rhs.e[2],
                lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0]);
}

inline auto unit_vector(const vec3& v)
{
    return v / v.length();
}
