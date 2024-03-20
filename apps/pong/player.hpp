#pragma once

#ifndef PONG_PLAYER_HPP_INCLUDED
#define PONG_PLAYER_HPP_INCLUDED

#include <borov_engine/box_component.hpp>
#include <borov_engine/input_key.hpp>

#include "direction.hpp"

class Player : public borov_engine::BoxComponent {
  public:
    struct ControlKeys {
        borov_engine::InputKey up;
        borov_engine::InputKey down;
    };

    explicit Player(borov_engine::Game &game, Direction direction, ControlKeys controls);

    [[nodiscard]] Direction Direction() const;
    [[nodiscard]] ControlKeys Controls() const;

    void Update(float delta_time) override;

  private:
    ::Direction direction_;
    ControlKeys controls_;

    static borov_engine::math::Vector3 PositionFrom(::Direction direction);
};

#endif //PONG_PLAYER_HPP_INCLUDED
