#include "game.hpp"

#include <numbers>

#include "camera_manager.hpp"
#include "viewport_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      sun_{CreateSun()},
      sun_mesh_{CreateSunMesh()},
      mercury_{CreateMercury()},
      mercury_mesh_{CreateMercuryMesh()},
      venus_{CreateVenus()},
      venus_mesh_{CreateVenusMesh()},
      earth_{CreateEarth()},
      earth_mesh_{CreateEarthMesh()},
      moon_{CreateMoon()},
      moon_mesh_{CreateMoonMesh()},
      mars_{CreateMars()},
      mars_mesh_{CreateMarsMesh()},
      phobos_{CreatePhobos()},
      phobos_mesh_{CreatePhobosMesh()},
      deimos_{CreateDeimos()},
      deimos_mesh_{CreateDeimosMesh()},
      jupyter_{CreateJupyter()},
      jupyter_mesh_{CreateJupyterMesh()},
      saturn_{CreateSaturn()},
      saturn_mesh_{CreateSaturnMesh()},
      uranus_{CreateUranus()},
      uranus_mesh_{CreateUranusMesh()},
      neptune_{CreateNeptune()},
      neptune_mesh_{CreateNeptuneMesh()} {
    CameraManager<::CameraManager>(0.0f, -std::numbers::pi_v<float> / 4.0f);
    ViewportManager<::ViewportManager>();

    if (borov_engine::Camera *camera = MainCamera()) {
        camera->Transform().position = borov_engine::math::Vector3{0.0f, 6.0f, 6.0f};
    }
}

void Game::Update(const float delta_time) {
    namespace math = borov_engine::math;

    borov_engine::Game::Update(delta_time);

    // Rotate the Mercury around the Sun
    const auto mercury_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    mercury_.Transform().RotateAround(math::Vector3::Zero, mercury_around_sun);

    // Rotate the Venus around the Sun
    const auto venus_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 1.17f * delta_time);
    venus_.Transform().RotateAround(math::Vector3::Zero, venus_around_sun);

    // Rotate the Venus around itself
    const auto venus_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Right, 2.0f * delta_time);
    auto &venus_mesh_rotation = venus_mesh_.Transform().rotation;
    venus_mesh_rotation = math::Quaternion::Concatenate(venus_mesh_rotation, venus_around_self);

    // Rotate the Moon around the Earth
    const auto moon_around_earth = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.5f * delta_time);
    moon_.Transform().RotateAround(math::Vector3::Zero, moon_around_earth);

    // Rotate the Moon around itself
    const auto moon_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    auto &moon_mesh_rotation = moon_mesh_.Transform().rotation;
    moon_mesh_rotation = math::Quaternion::Concatenate(moon_mesh_rotation, moon_around_self);

    // Rotate the Earth around the Sun
    const auto earth_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.25f * delta_time);
    earth_.Transform().RotateAround(math::Vector3::Zero, earth_around_sun);

    // Rotate the Earth around itself
    const auto earth_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    auto &earth_mesh_rotation = earth_mesh_.Transform().rotation;
    earth_mesh_rotation = math::Quaternion::Concatenate(earth_mesh_rotation, earth_around_self);

    // Rotate the Mars around the Sun
    const auto mars_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.25f * delta_time);
    mars_.Transform().RotateAround(math::Vector3::Zero, mars_around_sun);

    // Rotate the Deimos around the Mars
    const auto phobos_around_mars = math::Quaternion::CreateFromAxisAngle(math::Vector3::Forward, 1.5f * delta_time);
    phobos_.Transform().RotateAround(math::Vector3::Zero, phobos_around_mars);

    // Rotate the Deimos around the Mars
    const auto deimos_around_mars = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.5f * delta_time);
    deimos_.Transform().RotateAround(math::Vector3::Zero, deimos_around_mars);

    // Rotate the Mars around itself
    const auto mars_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Forward, 1.25f * delta_time);
    auto &mars_mesh_rotation = mars_mesh_.Transform().rotation;
    mars_mesh_rotation = math::Quaternion::Concatenate(mars_mesh_rotation, mars_around_self);

    // Rotate the Jupyter around the Sun
    const auto jupyter_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.12f * delta_time);
    jupyter_.Transform().RotateAround(math::Vector3::Zero, jupyter_around_sun);

    // Rotate the Saturn around the Sun
    const auto saturn_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.1f * delta_time);
    saturn_.Transform().RotateAround(math::Vector3::Zero, saturn_around_sun);

    // Rotate the Saturn around itself
    const auto saturn_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::One, 0.75f * delta_time);
    auto &saturn_mesh_rotation = saturn_mesh_.Transform().rotation;
    saturn_mesh_rotation = math::Quaternion::Concatenate(mars_mesh_rotation, saturn_around_self);

    // Rotate the Uranus around the Sun
    const auto uranus_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.07f * delta_time);
    uranus_.Transform().RotateAround(math::Vector3::Zero, uranus_around_sun);

    // Rotate the Neptune around the Sun
    const auto neptune_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.03f * delta_time);
    neptune_.Transform().RotateAround(math::Vector3::Zero, neptune_around_sun);
}

borov_engine::SceneComponent &Game::CreateSun() {
    return AddComponent<borov_engine::SceneComponent>();
}

borov_engine::GeometricPrimitiveComponent &Game::CreateSunMesh() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 2.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::Yellow};
    color *= 2.0f;
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &sun_, color);
}

borov_engine::SceneComponent &Game::CreateMercury() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 1.5f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMercuryMesh() {
    borov_engine::TorusGeometricPrimitiveArguments arguments{
        .diameter = 0.2f,
        .thickness = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::SlateGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &mercury_, color);
}

borov_engine::SceneComponent &Game::CreateVenus() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 2.0f,
        .rotation = borov_engine::math::Quaternion::CreateFromAxisAngle(borov_engine::math::Vector3::Right,
                                                                        std::numbers::pi_v<float> / 2.0f),
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateVenusMesh() {
    borov_engine::CylinderGeometricPrimitiveArguments arguments{
        .height = 0.25f,
        .diameter = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightYellow};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &venus_, color);
}

borov_engine::SceneComponent &Game::CreateEarth() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 3.0f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateEarthMesh() {
    borov_engine::GeoSphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::SpringGreen};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &earth_, color);
}

borov_engine::SceneComponent &Game::CreateMoon() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 0.5f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &earth_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMoonMesh() {
    borov_engine::TetrahedronGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGray};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &moon_, color);
}

borov_engine::SceneComponent &Game::CreateMars() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 4.5f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateMarsMesh() {
    borov_engine::IcosahedronGeometricPrimitiveArguments arguments{
        .size = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::OrangeRed};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &mars_, color);
}

borov_engine::SceneComponent &Game::CreatePhobos() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 0.5f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &mars_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreatePhobosMesh() {
    borov_engine::TeapotGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::PaleVioletRed};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &phobos_, color);
}

borov_engine::SceneComponent &Game::CreateDeimos() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitZ * 0.75f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &mars_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateDeimosMesh() {
    borov_engine::BoxGeometricPrimitiveArguments arguments{
        .size = borov_engine::math::Vector3::One * 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkRed};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &deimos_, color);
}

borov_engine::SceneComponent &Game::CreateJupyter() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 6.5f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateJupyterMesh() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 1.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkOrange};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &jupyter_, color);
}

borov_engine::SceneComponent &Game::CreateSaturn() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 8.0f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateSaturnMesh() {
    borov_engine::DodecahedronGeometricPrimitiveArguments arguments{
        .size = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGoldenrodYellow};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &saturn_, color);
}

borov_engine::SceneComponent &Game::CreateUranus() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 12.0f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateUranusMesh() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightSkyBlue};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &uranus_, color);
}

borov_engine::SceneComponent &Game::CreateNeptune() {
    borov_engine::Transform transform{
        .position = borov_engine::math::Vector3::UnitX * 18.0f,
    };
    return AddComponent<borov_engine::SceneComponent>(transform, &sun_);
}

borov_engine::GeometricPrimitiveComponent &Game::CreateNeptuneMesh() {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 0.75f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::BlueViolet};
    return AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, &neptune_, color);
}
