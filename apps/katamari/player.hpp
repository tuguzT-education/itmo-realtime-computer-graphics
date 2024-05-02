#pragma once

#ifndef KATAMARI_PLAYER_HPP_INCLUDED
#define KATAMARI_PLAYER_HPP_INCLUDED

#include <borov_engine/collision_primitive.hpp>
#include <borov_engine/geometric_primitive_component.hpp>
#include <borov_engine/input.hpp>

class Player final : public borov_engine::GeometricPrimitiveComponent, public borov_engine::CollisionPrimitive {
  public:
    struct ControlKeys {
        using enum borov_engine::InputKey;

        borov_engine::InputKey left = A;
        borov_engine::InputKey right = D;
        borov_engine::InputKey forward = W;
        borov_engine::InputKey backward = S;
    };

    explicit Player(borov_engine::Game& game, ControlKeys control_keys = {});

    [[nodiscard]] ControlKeys Controls() const;
    [[nodiscard]] ControlKeys& Controls();

    [[nodiscard]] bool Intersects(const CollisionPrimitive& other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] borov_engine::SphereCollisionPrimitive CollisionPrimitive() const;

    ControlKeys control_keys_;
};

#endif  // KATAMARI_PLAYER_HPP_INCLUDED
