#pragma once

#include "Hittable.hpp"

#include <memory>
#include <vector>

class HittableList : public Hittable {
    std::vector<std::unique_ptr<Hittable>> m_objects;

public:
    HittableList() = default;
    HittableList(std::unique_ptr<Hittable> object);

    void clear();
    void add(std::unique_ptr<Hittable> object);

    [[nodiscard]] auto hit(const Ray& ray, float t_min, float t_max) const -> std::optional<HitRecord> override;
};
