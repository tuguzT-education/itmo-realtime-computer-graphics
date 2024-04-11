#pragma once

#ifndef PONG_BALL_HPP_INCLUDED
#define PONG_BALL_HPP_INCLUDED

#include <borov_engine/box_component.hpp>

class Ball final : public borov_engine::BoxComponent {
  public:
    explicit Ball(borov_engine::Game &game);

    void Reset();

    void Update(float delta_time) override;

  private:
    [[nodiscard]] static borov_engine::math::Vector3 RandomVelocity();

    borov_engine::math::Vector3 velocity_;
};

#endif  // PONG_BALL_HPP_INCLUDED
