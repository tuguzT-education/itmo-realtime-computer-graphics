#include "game.hpp"

#include <numbers>

#include "camera_manager.hpp"
#include "viewport_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      sun_{CreateSun()},
      mercury_{CreateMercury()},
      venus_{CreateVenus()},
      earth_{CreateEarth()},
      moon_{CreateMoon()} {
    CameraManager<::CameraManager>();
    ViewportManager<::ViewportManager>();

    if (borov_engine::Camera *camera = MainCamera()) {
        camera->Position() = borov_engine::math::Vector3::Backward * 3.0f;
    }
}

void Game::Update(const float delta_time) {
    namespace math = borov_engine::math;

    borov_engine::Game::Update(delta_time);

    const auto rotate_mercury_by = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    mercury_.Transform().RotateAround(sun_.Transform().position, rotate_mercury_by);

    const auto rotate_venus_by = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 1.17f * delta_time);
    venus_.Transform().RotateAround(sun_.Transform().position, rotate_venus_by);

    const auto rotate_moon_by = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.5f * delta_time);
    moon_.Transform().RotateAround(earth_.Transform().position, rotate_moon_by);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateSun() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 2.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::Yellow};
    color *= 2.0f;
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMercury() {
    borov_engine::TorusGeometricPrimitiveArguments arguments{
        .diameter = 0.2f,
        .thickness = 0.1f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 1.5f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::SlateGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateVenus() {
    borov_engine::CylinderGeometricPrimitiveArguments arguments{
        .height = 0.25f,
        .diameter = 0.25f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 3.0f,
        .rotation = borov_engine::math::Quaternion::CreateFromAxisAngle(borov_engine::math::Vector3::Right,
                                                                        std::numbers::pi_v<float> / 2.0f),
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightYellow};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateEarth() {
    borov_engine::GeoSphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 5.0f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::SpringGreen};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMoon() {
    borov_engine::TetrahedronGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 5.5f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, color);
}
