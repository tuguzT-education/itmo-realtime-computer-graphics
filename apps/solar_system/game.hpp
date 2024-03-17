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
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateVenus();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateEarth();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateMoon();

    borov_engine::GeometricPrimitiveComponent &sun_;
    borov_engine::GeometricPrimitiveComponent &mercury_;
    borov_engine::GeometricPrimitiveComponent &venus_;
    borov_engine::GeometricPrimitiveComponent &earth_;
    borov_engine::GeometricPrimitiveComponent &moon_;
};

#endif //SOLAR_SYSTEM_GAME_HPP_INCLUDED
