#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED

#include <borov_engine/collision_primitive.hpp>
#include <borov_engine/geometric_primitive_component.hpp>

class Deimos final : public borov_engine::SceneComponent, public borov_engine::CollisionPrimitive {
  public:
    explicit Deimos(borov_engine::Game &game, const borov_engine::Transform &transform = {},
                    const SceneComponent *parent = nullptr);

    [[nodiscard]] const borov_engine::GeometricPrimitiveComponent &Mesh() const;
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &Mesh();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray &ray, float &dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollisionPrimitive CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::GeometricPrimitiveComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_DEIMOS_HPP_INCLUDED
