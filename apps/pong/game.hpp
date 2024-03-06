#pragma once

#ifndef PONG_GAME_HPP_INCLUDED
#define PONG_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

#include "ball.hpp"

class Game : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

    void Update(float delta_time) override;

  private:
    Ball &ball_;
    std::size_t left_score_;
    std::size_t right_score_;
};

#endif //PONG_GAME_HPP_INCLUDED
