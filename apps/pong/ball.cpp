#include "ball.hpp"

#include <borov_engine/game.hpp>
#include <random>

#include "player.hpp"

Ball::Ball(borov_engine::Game &game) : BoxComponent(game, 0.05f, 0.05f, 0.05f), velocity_{RandomVelocity()} {}

void Ball::Reset() {
    Transform().position = borov_engine::math::Vector3::Zero;
    velocity_ = RandomVelocity();
}

void Ball::Update(const float delta_time) {
    velocity_ += borov_engine::math::Normalize(velocity_) * (0.05f * delta_time);

    auto &position = Transform().position;
    if (position.x < -0.975f || position.x > 0.975f) {
        velocity_.x = -velocity_.x;
    }
    if (position.y < -0.975f || position.y > 0.975f) {
        velocity_.y = -velocity_.y;
    }

    for (const Component &component : Game().Components()) {
        const auto player = dynamic_cast<const Player *>(&component);
        if (player == nullptr) {
            continue;
        }

        if (player->Intersects(*this)) {
            velocity_.x = -velocity_.x;
        }
    }

    position += velocity_ * delta_time;
}

borov_engine::math::Vector3 Ball::RandomVelocity() {
    static std::random_device device;
    static std::default_random_engine engine{device()};
    static std::uniform_real_distribution distribution{-1.0f, 1.0f};

    namespace math = borov_engine::math;

    const float temp = distribution(engine);
    const float x = temp + (temp > 0.0f ? 1.0f : -1.0f);
    const float y = distribution(engine);
    return math::Normalize(math::Vector3{x, y, 0.0f});
}
