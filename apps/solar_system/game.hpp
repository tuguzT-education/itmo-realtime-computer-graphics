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

    [[nodiscard]] borov_engine::SceneComponent &CreateMars();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateMarsMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreatePhobos();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreatePhobosMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateDeimos();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateDeimosMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateJupyter();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateJupyterMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateSaturn();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateSaturnMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateUranus();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateUranusMesh();

    [[nodiscard]] borov_engine::SceneComponent &CreateNeptune();
    [[nodiscard]] borov_engine::GeometricPrimitiveComponent &CreateNeptuneMesh();

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

    borov_engine::SceneComponent &mars_;
    borov_engine::GeometricPrimitiveComponent &mars_mesh_;

    borov_engine::SceneComponent &phobos_;
    borov_engine::GeometricPrimitiveComponent &phobos_mesh_;

    borov_engine::SceneComponent &deimos_;
    borov_engine::GeometricPrimitiveComponent &deimos_mesh_;

    borov_engine::SceneComponent &jupyter_;
    borov_engine::GeometricPrimitiveComponent &jupyter_mesh_;

    borov_engine::SceneComponent &saturn_;
    borov_engine::GeometricPrimitiveComponent &saturn_mesh_;

    borov_engine::SceneComponent &uranus_;
    borov_engine::GeometricPrimitiveComponent &uranus_mesh_;

    borov_engine::SceneComponent &neptune_;
    borov_engine::GeometricPrimitiveComponent &neptune_mesh_;
};

#endif  // SOLAR_SYSTEM_GAME_HPP_INCLUDED
