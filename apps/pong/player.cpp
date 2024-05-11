#include "player.hpp"

#include <borov_engine/game.hpp>

Player::Player(borov_engine::Game &game, const ::Direction direction, const ControlKeys controls)
    : BoxComponent(
          game, reinterpret_cast<Initializer &>(Initializer{.length = 0.05f, .height = 0.3f, .width = 0.05f}.Transform(
                    borov_engine::Transform{.position = PositionFrom(direction)}))),
      direction_{direction},
      controls_{controls} {}

Direction Player::Direction() const {
    return direction_;
}

Player::ControlKeys Player::Controls() const {
    return controls_;
}

void Player::Update(const float delta_time) {
    namespace math = borov_engine::math;

    const auto *input = Game().Input();
    if (input == nullptr) {
        return;
    }

    auto &position = Transform().position;
    const auto y = static_cast<float>(input->IsKeyDown(controls_.up) - input->IsKeyDown(controls_.down));
    const math::Vector3 movement = math::Vector3::Up * y;
    constexpr float speed = 2.0f;
    position += speed * movement * delta_time;

    if (position.y > 0.85f) {
        position.y = 0.85f;
    } else if (position.y < -0.85f) {
        position.y = -0.85f;
    }
}

borov_engine::math::Vector3 Player::PositionFrom(const ::Direction direction) {
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
