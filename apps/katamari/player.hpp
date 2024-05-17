#pragma once

#ifndef KATAMARI_PLAYER_HPP_INCLUDED
#define KATAMARI_PLAYER_HPP_INCLUDED

#include <borov_engine/collision.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/mesh_component.hpp>

#include "player_child.hpp"

class Player final : public borov_engine::MeshComponent<PlayerChild>, public borov_engine::Collision {
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

    void Draw(const borov_engine::Camera* camera) override;

    [[nodiscard]] bool Intersects(const Collision& other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] borov_engine::SphereCollision CollisionPrimitive() const;

    ControlKeys control_keys_;
};

#endif  // KATAMARI_PLAYER_HPP_INCLUDED
