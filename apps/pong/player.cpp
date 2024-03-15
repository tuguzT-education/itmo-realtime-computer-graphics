#include "player.hpp"

Player::Player(borov_engine::Game &game, ::Direction direction, ControlKeys controls)
    : BoxComponent(game, 0.05f, 0.3f, 0.05f, {1.0f, 1.0f, 1.0f}, {.position = PositionFrom(direction)}),
      direction_{direction},
      controls_{controls} {}

Direction Player::Direction() const {
    return direction_;
}

Player::ControlKeys Player::Controls() const {
    return controls_;
}

void Player::Update(float delta_time) {
    namespace math = borov_engine::math;

    auto *input = Game().Input();
    if (input == nullptr) {
        return;
    }

    auto &position = Transform().position;
    auto y = static_cast<float>(input->IsKeyDown(controls_.up) - input->IsKeyDown(controls_.down));
    math::Vector3 movement = math::Vector3::Up * y;
    float speed = 2.0f;
    position += speed * movement * delta_time;

    if (position.y > 0.85f) {
        position.y = 0.85f;
    } else if (position.y < -0.85f) {
        position.y = -0.85f;
    }
}

borov_engine::math::Vector3 Player::PositionFrom(::Direction direction) {
    switch (direction) {
        case Direction::Left: {
            return {-0.975f, 0.0f, 0.0f};
        }
        case Direction::Right: {
            return {0.975f, 0.0f, 0.0f};
        }
        default: {
            return {};
        }
    }
}
