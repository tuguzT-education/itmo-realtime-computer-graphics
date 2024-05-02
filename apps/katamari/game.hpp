#pragma once

#ifndef KATAMARI_GAME_HPP_INCLUDED
#define KATAMARI_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

#include "player.hpp"

class Game final : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  private:
    std::reference_wrapper<borov_engine::Camera> camera_;
    std::reference_wrapper<Player> player_;
};

#endif  // KATAMARI_GAME_HPP_INCLUDED
