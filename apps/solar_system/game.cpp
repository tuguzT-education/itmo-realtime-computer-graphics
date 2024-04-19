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
    CameraManager<class CameraManager>();
    ViewportManager<class ViewportManager>();

    if (borov_engine::Camera *camera = MainCamera()) {
        camera->Position() = borov_engine::math::Vector3::Backward * 3.0f;
    }
}

void Game::Update(const float delta_time) {
    namespace math = borov_engine::math;

    borov_engine::Game::Update(delta_time);

    const auto mercury_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    mercury_.Transform().RotateAround(math::Vector3::Zero, mercury_around_sun);

    const auto venus_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 1.17f * delta_time);
    venus_.Transform().RotateAround(math::Vector3::Zero, venus_around_sun);

    const auto moon_around_earth = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.5f * delta_time);
    moon_.Transform().RotateAround(math::Vector3::Zero, moon_around_earth);

    // const auto earth_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    // earth_.Transform().rotation = math::Quaternion::Concatenate(earth_.Transform().rotation, earth_around_self);
    const auto earth_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.25f * delta_time);
    earth_.Transform().RotateAround(math::Vector3::Zero, earth_around_sun);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateSun() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 2.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::Yellow};
    color *= 2.0f;
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, nullptr, color);
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
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, nullptr, color);
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
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &sun_, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateEarth() {
    borov_engine::GeoSphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 5.0f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::SpringGreen};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &sun_, color);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMoon() {
    borov_engine::TetrahedronGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 0.5f,
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &earth_, color);
}
