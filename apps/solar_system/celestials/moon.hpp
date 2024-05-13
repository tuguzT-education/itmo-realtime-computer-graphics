#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_MOON_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_MOON_HPP_INCLUDED

#include <borov_engine/collision_primitive.hpp>
#include <borov_engine/geometric_primitive_component.hpp>

class Moon final : public borov_engine::SceneComponent, public borov_engine::CollisionPrimitive {
  public:
    explicit Moon(borov_engine::Game &game, const Initializer &initializer = {});

    [[nodiscard]] const borov_engine::GeometricPrimitiveComponent &Mesh() const;
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &Mesh();

    void Draw(const borov_engine::Camera *camera) override;

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray &ray, float &dist) const override;

  private:
    [[nodiscard]] borov_engine::SphereCollisionPrimitive CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::GeometricPrimitiveComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_MOON_HPP_INCLUDED
