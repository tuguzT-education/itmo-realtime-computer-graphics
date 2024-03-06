#pragma once

#ifndef EXAMPLE_GAME_HPP_INCLUDED
#define EXAMPLE_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

class Game final : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  protected:
    void Update(float delta_time) override;
    void Draw() override;

  private:
    std::string initial_title_;
};

#endif //EXAMPLE_GAME_HPP_INCLUDED
