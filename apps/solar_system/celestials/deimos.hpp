#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED

#include <borov_engine/collision.hpp>
#include <borov_engine/geometric_primitive_component.hpp>

class Deimos final : public borov_engine::SceneComponent, public borov_engine::Collision {
  public:
    explicit Deimos(borov_engine::Game &game, const Initializer &initializer = {});

    [[nodiscard]] const borov_engine::GeometricPrimitiveComponent &Mesh() const;
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &Mesh();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray &ray, float &dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollision CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::GeometricPrimitiveComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED
