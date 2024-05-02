#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED

#include <borov_engine/box_component.hpp>
#include <borov_engine/collision_primitive.hpp>

class Earth final : public borov_engine::SceneComponent, public borov_engine::CollisionPrimitive {
  public:
    explicit Earth(borov_engine::Game &game, const borov_engine::Transform &transform = {},
                   const SceneComponent *parent = nullptr);

    [[nodiscard]] const borov_engine::BoxComponent &Mesh() const;
    [[nodiscard]] borov_engine::BoxComponent &Mesh();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray &ray, float &dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollisionPrimitive CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::BoxComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED
