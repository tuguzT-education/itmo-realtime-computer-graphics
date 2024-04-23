#pragma once

#ifndef SOLAR_SYSTEM_GAME_HPP_INCLUDED
#define SOLAR_SYSTEM_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>

#include "celestials/deimos.hpp"
#include "celestials/earth.hpp"
#include "celestials/jupyter.hpp"
#include "celestials/mars.hpp"
#include "celestials/mercury.hpp"
#include "celestials/moon.hpp"
#include "celestials/neptune.hpp"
#include "celestials/phobos.hpp"
#include "celestials/saturn.hpp"
#include "celestials/sun.hpp"
#include "celestials/uranus.hpp"
#include "celestials/venus.hpp"

class Game final : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  protected:
    void Update(float delta_time) override;

  private:
    void SwitchCameraManagerIfNeeded();
    void MovePlanets(float delta_time);

    borov_engine::Camera &camera_;

    Sun &sun_;
    Mercury &mercury_;
    Venus &venus_;
    Earth &earth_;
    Moon &moon_;
    Mars &mars_;
    Phobos &phobos_;
    Deimos &deimos_;
    Jupyter &jupyter_;
    Saturn &saturn_;
    Uranus &uranus_;
    Neptune &neptune_;
};

#endif  // SOLAR_SYSTEM_GAME_HPP_INCLUDED
