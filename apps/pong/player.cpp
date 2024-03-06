#include "player.hpp"

Player::Player(borov_engine::Game &game, ::Direction direction, ControlKeys controls)
    : BoxComponent(game, 0.05f, 0.3f, 0.05f, {1.0f, 1.0f, 1.0f}, PositionFrom(direction)),
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
            return {-0.9f, 0.0f, 0.0f};
        }
        case Direction::Right: {
            return {0.9f, 0.0f, 0.0f};
        }
        default: {
            return {};
        }
    }
}
