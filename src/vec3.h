#pragma once

#include <cmath>
#include <array>
#include <iostream>

class vec3
{
    std::array<double, 3> e{};

public:
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
