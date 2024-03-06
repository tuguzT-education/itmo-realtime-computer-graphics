#pragma once

#ifndef PONG_BALL_HPP_INCLUDED
#define PONG_BALL_HPP_INCLUDED

#include "player.hpp"

#include <random>

class Ball : public borov_engine::BoxComponent {
  public:
    explicit Ball(borov_engine::Game &game);

    void Reset();

    void Update(float delta_time) override;

  private:
    [[nodiscard]] static borov_engine::math::Vector3 RandomVelocity();

    borov_engine::math::Vector3 velocity_;
};

Ball::Ball(borov_engine::Game &game)
    : BoxComponent(game, 0.05f, 0.05f, 0.0f),
      velocity_{RandomVelocity()} {}

void Ball::Reset() {
    Position() = borov_engine::math::Vector3{};
    velocity_ = RandomVelocity();
}

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

    for (const Component &component : Components()) {
        auto player = dynamic_cast<const Player *>(&component);
        if (player == nullptr) {
            continue;
        }

        Box box = Collision();
        Box player_box = player->Collision();
        if (box.Intersects(player_box)) {
            velocity_.x = -velocity_.x;
        }
    }

    position += velocity_ * delta_time;
}

borov_engine::math::Vector3 Ball::RandomVelocity() {
    static std::random_device device;
    static std::default_random_engine engine{device()};
    static std::uniform_real_distribution distribution{-1.0f, 1.0f};

    float x = distribution(engine);
    x += (x < 0) ? -1 : 1;
    float y = distribution(engine);
    borov_engine::math::Vector3 vector{x, y, 0.0f};
    vector.Normalize();
    return vector;
}

#endif //PONG_BALL_HPP_INCLUDED
