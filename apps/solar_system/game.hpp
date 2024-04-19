#pragma once

#ifndef SOLAR_SYSTEM_GAME_HPP_INCLUDED
#define SOLAR_SYSTEM_GAME_HPP_INCLUDED

#include <borov_engine/game.hpp>
#include <borov_engine/geometric_primitive_component.hpp>

class Game final : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input);

  protected:
    void Update(float delta_time) override;

  private:
    [[nodiscard]] borov_engine::SceneComponent &CreateSun();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateSunMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateMercury();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateMercuryMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateVenus();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateVenusMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateEarth();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateEarthMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateMoon();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateMoonMesh();

    borov_engine::SceneComponent &sun_;
    borov_engine::GeometricPrimitiveComponent &sun_mesh_;

    borov_engine::SceneComponent &mercury_;
    borov_engine::GeometricPrimitiveComponent &mercury_mesh_;

    borov_engine::SceneComponent &venus_;
    borov_engine::GeometricPrimitiveComponent &venus_mesh_;

    borov_engine::SceneComponent &earth_;
    borov_engine::GeometricPrimitiveComponent &earth_mesh_;

    borov_engine::SceneComponent &moon_;
    borov_engine::GeometricPrimitiveComponent &moon_mesh_;
};

#endif  // SOLAR_SYSTEM_GAME_HPP_INCLUDED
