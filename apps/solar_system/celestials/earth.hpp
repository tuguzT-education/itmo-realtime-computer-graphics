#pragma once

#ifndef SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED
#define SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED

#include <borov_engine/geometric_primitive_component.hpp>

#include "../collision.hpp"
#include "borov_engine/box_component.hpp"

class Earth final : public borov_engine::SceneComponent, public Collision {
  public:
    explicit Earth(borov_engine::Game &game, const borov_engine::Transform &transform = {},
                   const SceneComponent *parent = nullptr);

    [[nodiscard]] const borov_engine::BoxComponent &Mesh() const;
    [[nodiscard]] borov_engine::BoxComponent &Mesh();

    [[nodiscard]] borov_engine::math::AxisAlignedBox BoxCollision() const override;

  private:
    std::reference_wrapper<borov_engine::BoxComponent> mesh_;
};

#endif  // SOLAR_SYSTEM_CELESTIALS_EARTH_HPP_INCLUDED
