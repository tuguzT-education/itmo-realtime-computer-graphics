#pragma once

#ifndef KATAMARI_GAME_HPP_INCLUDED
#define KATAMARI_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

#include "field.hpp"
#include "objects/apricot.hpp"
#include "objects/axe.hpp"
#include "objects/bulb.hpp"
#include "objects/hog.hpp"
#include "player.hpp"

class Game final : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

    void Update(float delta_time) override;

  private:
    std::reference_wrapper<borov_engine::Camera> camera_;
    std::reference_wrapper<Field> field_;
    std::reference_wrapper<Player> player_;

    std::reference_wrapper<Apricot> apricot_;
    std::reference_wrapper<Axe> axe_;
    std::reference_wrapper<Bulb> bulb_;
    std::reference_wrapper<Hog> hog_;
};

#endif  // KATAMARI_GAME_HPP_INCLUDED
