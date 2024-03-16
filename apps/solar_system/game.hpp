#pragma once

#ifndef SOLAR_SYSTEM_GAME_HPP_INCLUDED
#define SOLAR_SYSTEM_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>
#include <borov_engine/geometric_primitive_component.hpp>

class Game : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  protected:
    void Update(float delta_time) override;

  private:
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateSun();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateMercury();

    borov_engine::GeometricPrimitiveComponent &sun_;
    borov_engine::GeometricPrimitiveComponent &mercury_;
};

#endif //SOLAR_SYSTEM_GAME_HPP_INCLUDED
