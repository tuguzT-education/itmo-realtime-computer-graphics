#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_SUN_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_SUN_HPP_INCLUDED

#include <borov_engine/box_component.hpp>
#include <borov_engine/collision.hpp>

class Sun final : public borov_engine::SceneComponent, public borov_engine::Collision {
  public:
    explicit Sun(borov_engine::Game &game, const Initializer &initializer = {});

    [[nodiscard]] const borov_engine::BoxComponent &Mesh() const;
    [[nodiscard]] borov_engine::BoxComponent &Mesh();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray &ray, float &dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollision CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::BoxComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_SUN_HPP_INCLUDED
