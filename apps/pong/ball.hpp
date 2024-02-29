#pragma once

#ifndef PONG_BALL_HPP_INCLUDED
#define PONG_BALL_HPP_INCLUDED

#include <borov_engine/box_component.hpp>

class Ball : public borov_engine::BoxComponent {
  public:
    explicit Ball(borov_engine::Game &game);

    void Update(float delta_time) override;

  private:
    borov_engine::math::Vector3 velocity_;
};

Ball::Ball(borov_engine::Game &game)
    : BoxComponent(game, 0.05f, 0.05f, borov_engine::math::Color{1.0f, 1.0f, 1.0f}),
      velocity_{0.5f, 0.4f, 0.0f} {}

void Ball::Update(float delta_time) {
    borov_engine::math::Vector3 normal;
    velocity_.Normalize(normal);
    velocity_ += normal * (0.05f * delta_time);

    auto &position = Position();
    if (position.x < -1.0f || position.x > 1.0f) {
        velocity_.x = -velocity_.x;
    }
    if (position.y < -1.0f || position.y > 1.0f) {
        velocity_.y = -velocity_.y;
    }

    position += velocity_ * delta_time;
}

#endif //PONG_BALL_HPP_INCLUDED
