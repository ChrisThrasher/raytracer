#pragma once

#include "Vec3.h"

auto RandomInUnitSphere()
{
    while (true) {
        const auto p = Vec3::Random(-1, 1);
        if (p.Length2() >= 1)
            continue;
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
    if (Dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

auto RandomInUnitDisk()
{
    while (true) {
        const auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if (p.Length2() >= 1)
            continue;
        return p;
    }
}
