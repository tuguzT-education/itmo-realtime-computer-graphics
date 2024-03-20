#include "ball.hpp"

#include <random>
#include <borov_engine/game.hpp>

#include "player.hpp"

Ball::Ball(borov_engine::Game &game)
    : BoxComponent(game, 0.05f, 0.05f, 0.05f),
      velocity_{RandomVelocity()} {}

void Ball::Reset() {
    Transform().position = borov_engine::math::Vector3::Zero;
    velocity_ = RandomVelocity();
}

void Ball::Update(float delta_time) {
    borov_engine::math::Vector3 normal;
    velocity_.Normalize(normal);
    velocity_ += normal * (0.05f * delta_time);

    auto &position = Transform().position;
    if (position.x < -0.975f || position.x > 0.975f) {
        velocity_.x = -velocity_.x;
    }
    if (position.y < -0.975f || position.y > 0.975f) {
        velocity_.y = -velocity_.y;
    }

    for (const Component &component : Game().Components()) {
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
    x += x > 0.0f ? 1.0f : -1.0f;
    float y = distribution(engine);
    borov_engine::math::Vector3 vector{x, y, 0.0f};
    vector.Normalize();
    return vector;
}
