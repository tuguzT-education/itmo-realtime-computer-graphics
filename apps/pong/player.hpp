#pragma once

#ifndef PONG_LEFT_PLAYER_HPP_INCLUDED
#define PONG_LEFT_PLAYER_HPP_INCLUDED

#include <borov_engine/box_component.hpp>

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

Player::Player(borov_engine::Game &game, ::Direction direction, ControlKeys controls)
    : BoxComponent(game, 0.05f, 0.3f, borov_engine::math::Color{1.0f, 1.0f, 1.0f}, PositionFrom(direction)),
      direction_{direction},
      controls_{controls} {}

Direction Player::Direction() const {
    return direction_;
}

Player::ControlKeys Player::Controls() const {
    return controls_;
}

void Player::Update(float delta_time) {
    auto *input = Input();
    if (input == nullptr) {
        return;
    }

    auto &position = Position();
    if (input->IsKeyDown(controls_.up)) {
        position += borov_engine::math::Vector3::Up * (2.0f * delta_time);
    }
    if (input->IsKeyDown(controls_.down)) {
        position += borov_engine::math::Vector3::Down * (2.0f * delta_time);
    }

    if (position.y > 0.85f) {
        position.y = 0.85f;
    } else if (position.y < -0.85f) {
        position.y = -0.85f;
    }
}

borov_engine::math::Vector3 Player::PositionFrom(::Direction direction) {
    switch (direction) {
        case Direction::Left: {
            return {-0.95f, 0.0f, 0.0f};
        }
        case Direction::Right: {
            return {0.95f, 0.0f, 0.0f};
        }
    }
}

#endif //PONG_LEFT_PLAYER_HPP_INCLUDED
