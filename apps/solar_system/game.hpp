#pragma once

#ifndef SOLAR_SYSTEM_GAME_HPP_INCLUDED
#define SOLAR_SYSTEM_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

class Game : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  protected:
    void OnTargetResize() override;

  private:
    void SetupViewports();
};

#endif //SOLAR_SYSTEM_GAME_HPP_INCLUDED
