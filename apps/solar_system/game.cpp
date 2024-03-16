#include "game.hpp"

#include <array>

#include "camera_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      sun_{CreateSun()},
      mercury_{CreateMercury()} {
    CameraManager<::CameraManager>();
    Viewports({});
}

void Game::Update(float delta_time) {
    borov_engine::Game::Update(delta_time);

    namespace math = borov_engine::math;

    auto rotate_mercury_by = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 1.0f * delta_time);
    mercury_.Transform().RotateAround(sun_.Transform().position, rotate_mercury_by);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateSun() {
    borov_engine::SphereGeometricPrimitiveArguments sphere_arguments;
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::Yellow};
    color *= 2.0f;
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(sphere_arguments, transform, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMercury() {
    borov_engine::TorusGeometricPrimitiveArguments torus_arguments{
        .diameter = 0.1f,
        .thickness = 0.1f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 0.75f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::SlateGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(torus_arguments, transform, color);
}
