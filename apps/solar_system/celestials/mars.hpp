#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_MARS_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_MARS_HPP_INCLUDED

#include <borov_engine/geometric_primitive_component.hpp>

#include "../collision.hpp"

class Mars final : public borov_engine::SceneComponent, public Collision {
  public:
    explicit Mars(borov_engine::Game &game, const borov_engine::Transform &transform = {},
                  const SceneComponent *parent = nullptr);

    [[nodiscard]] const borov_engine::GeometricPrimitiveComponent &Mesh() const;
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &Mesh();

    [[nodiscard]] borov_engine::math::AxisAlignedBox BoxCollision() const override;

  private:
    std::reference_wrapper<borov_engine::GeometricPrimitiveComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_MARS_HPP_INCLUDED
