#pragma once

#include "Hittable.hpp"

#include <memory>
#include <vector>

class HittableList : public Hittable {
    std::vector<std::shared_ptr<Hittable>> m_objects;

public:
    HittableList() = default;
    HittableList(const std::shared_ptr<Hittable>& object);

    void clear();
    void add(const std::shared_ptr<Hittable>& object);

    [[nodiscard]] auto hit(const Ray& ray, float t_min, float t_max) const -> std::optional<HitRecord> override;
};
