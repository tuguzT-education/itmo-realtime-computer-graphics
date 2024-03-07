#include "game.hpp"

#include <iostream>

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      ball_{AddComponent<Ball>()},
      left_score_{},
      right_score_{} {}

void Game::Update(float delta_time) {
    borov_engine::Game::Update(delta_time);

    const auto &position = ball_.Position();
    if (position.x < -0.975f) {
        std::cout << "One point to the RIGHT!";
        right_score_++;
    } else if (position.x > 0.975f) {
        std::cout << "One point to the LEFT!";
        left_score_++;
    } else {
        return;
    }

    ball_.Reset();
    std::cout << "\n\tLEFT:  " << left_score_
              << "\n\tRIGHT: " << right_score_
              << "\n\n";
}
