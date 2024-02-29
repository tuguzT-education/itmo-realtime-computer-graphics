#pragma once

#ifndef PONG_REFEREE_HPP_INCLUDED
#define PONG_REFEREE_HPP_INCLUDED

#include "ball.hpp"

#include <iostream>

class Referee : public borov_engine::Component {
  public:
    explicit Referee(borov_engine::Game &game);

    void Update(float delta_time) override;
    void Draw() override;

  private:
    std::size_t left_score_;
    std::size_t right_score_;
};

Referee::Referee(borov_engine::Game &game) : Component(game), left_score_{}, right_score_{} {}

void Referee::Update(float delta_time) {
    Ball *ball = nullptr;
    for (auto &component : Components()) {
        if (ball == nullptr) {
            ball = dynamic_cast<Ball *>(component.get());
        }
    }
    if (ball == nullptr) {
        return;
    }

    const auto &position = ball->Position();
    if (position.x < -1.0f) {
        right_score_++;
    } else if (position.x > 1.0f) {
        left_score_++;
    } else {
        return;
    }

    ball->Reset();
    std::cout << "Score:"
              << "\n\tLeft: " << left_score_
              << "\n\tRight: " << right_score_
              << std::endl;
}

void Referee::Draw() {}

#endif //PONG_REFEREE_HPP_INCLUDED
