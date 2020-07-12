#pragma once

class camera
{
    point3 origin{};
    point3 lower_left_corner{};
    vec3 horizontal{};
    vec3 vertical{};

public:
    constexpr camera()
    {
        constexpr auto aspect_ratio = 16.0 / 9.0;
        constexpr auto viewport_height = 2.0;
        constexpr auto viewport_width = aspect_ratio * viewport_height;
        constexpr auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin
                          - horizontal / 2
                          - vertical / 2
                          - vec3(0, 0, focal_length);
    }

    constexpr auto get_ray(const double u, const double v) const
    {
        return ray(origin,
                   lower_left_corner
                   + u * horizontal
                   + v * vertical
                   - origin);
    }
};
