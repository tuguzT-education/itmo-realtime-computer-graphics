#include "game.hpp"

#include <borov_engine/camera.hpp>
#include <borov_engine/collision_primitive.hpp>
#include <borov_engine/orbit_camera_manager.hpp>
#include <borov_engine/spectator_camera_manager.hpp>

#include "viewport_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      camera_{
          AddComponent<borov_engine::Camera>(borov_engine::PerspectiveCameraProjectionType{},
                                             borov_engine::Transform{
                                                 .position = borov_engine::math::Vector3{0.0f, 6.0f, 6.0f},
                                                 .rotation = borov_engine::math::Quaternion::CreateFromYawPitchRoll(
                                                     0.0f, -std::numbers::pi_v<float> / 4.0f, 0.0f),
                                             }),
      },
      sun_{
          AddComponent<Sun>(),
      },
      mercury_{
          AddComponent<Mercury>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 1.5f,
              },
              &sun_),
      },
      venus_{
          AddComponent<Venus>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 2.0f,
              },
              &sun_),
      },
      earth_{
          AddComponent<Earth>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 3.0f,
              },
              &sun_),
      },
      moon_{
          AddComponent<Moon>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 0.5f,
              },
              &earth_),
      },
      mars_{
          AddComponent<Mars>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 4.5f,
              },
              &sun_),
      },
      phobos_{
          AddComponent<Phobos>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 0.5f,
              },
              &mars_),
      },
      deimos_{
          AddComponent<Deimos>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitZ * 0.75f,
              },
              &mars_),
      },
      jupyter_{
          AddComponent<Jupyter>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 6.5f,
              },
              &sun_),
      },
      saturn_{
          AddComponent<Saturn>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 8.0f,
              },
              &sun_),
      },
      uranus_{
          AddComponent<Uranus>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 12.0f,
              },
              &sun_),
      },
      neptune_{
          AddComponent<Neptune>(
              borov_engine::Transform{
                  .position = borov_engine::math::Vector3::UnitX * 18.0f,
              },
              &sun_),
      } {
    CameraManager<borov_engine::SpectatorCameraManager>(camera_);
    ViewportManager<::ViewportManager>();

    input.OnInputKeyDown().AddRaw(this, &Game::OnInputKeyDown);
}

Game::~Game() {
    if (const auto input = Input(); input != nullptr) {
        input->OnInputKeyDown().RemoveByOwner(this);
    }
}

void Game::Update(const float delta_time) {
    borov_engine::Game::Update(delta_time);

    namespace math = borov_engine::math;

    // Rotate the Mercury around the Sun
    const auto mercury_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    mercury_.Transform().RotateAround(math::Vector3::Zero, mercury_around_sun);

    // Rotate the Venus around the Sun
    const auto venus_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 1.17f * delta_time);
    venus_.Transform().RotateAround(math::Vector3::Zero, venus_around_sun);

    // Rotate the Venus around itself
    const auto venus_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Right, 2.0f * delta_time);
    auto &venus_mesh_rotation = venus_.Mesh().Transform().rotation;
    venus_mesh_rotation = math::Quaternion::Concatenate(venus_mesh_rotation, venus_around_self);

    // Rotate the Moon around the Earth
    const auto moon_around_earth = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.5f * delta_time);
    moon_.Transform().RotateAround(math::Vector3::Zero, moon_around_earth);

    // Rotate the Moon around itself
    const auto moon_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2.0f * delta_time);
    auto &moon_mesh_rotation = moon_.Mesh().Transform().rotation;
    moon_mesh_rotation = math::Quaternion::Concatenate(moon_mesh_rotation, moon_around_self);

    // Rotate the Earth around the Sun
    const auto earth_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.25f * delta_time);
    earth_.Transform().RotateAround(math::Vector3::Zero, earth_around_sun);

    // Rotate the Earth around itself
    const auto earth_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 2000.0f * delta_time);
    auto &earth_mesh_rotation = earth_.Mesh().Transform().rotation;
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
    auto &mars_mesh_rotation = mars_.Mesh().Transform().rotation;
    mars_mesh_rotation = math::Quaternion::Concatenate(mars_mesh_rotation, mars_around_self);

    // Rotate the Jupyter around the Sun
    const auto jupyter_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.12f * delta_time);
    jupyter_.Transform().RotateAround(math::Vector3::Zero, jupyter_around_sun);

    // Rotate the Saturn around the Sun
    const auto saturn_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.1f * delta_time);
    saturn_.Transform().RotateAround(math::Vector3::Zero, saturn_around_sun);

    // Rotate the Saturn around itself
    const auto saturn_around_self = math::Quaternion::CreateFromAxisAngle(math::Vector3::One, 0.75f * delta_time);
    auto &saturn_mesh_rotation = saturn_.Mesh().Transform().rotation;
    saturn_mesh_rotation = math::Quaternion::Concatenate(mars_mesh_rotation, saturn_around_self);

    // Rotate the Uranus around the Sun
    const auto uranus_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.07f * delta_time);
    uranus_.Transform().RotateAround(math::Vector3::Zero, uranus_around_sun);

    // Rotate the Neptune around the Sun
    const auto neptune_around_sun = math::Quaternion::CreateFromAxisAngle(math::Vector3::Up, 0.03f * delta_time);
    neptune_.Transform().RotateAround(math::Vector3::Zero, neptune_around_sun);

    // Rotate the Sun around itself
    const math::Vector3 sun_axis = math::Normalize(math::Vector3::Right + math::Vector3::Forward);
    const auto sun_around_self = math::Quaternion::CreateFromAxisAngle(sun_axis, 2.0f * delta_time);
    auto &sun_mesh_rotation = sun_.Mesh().Transform().rotation;
    sun_mesh_rotation = math::Quaternion::Concatenate(sun_mesh_rotation, sun_around_self);
}

void Game::OnInputKeyDown(const borov_engine::InputKey input_key) {
    switch (input_key) {
        case borov_engine::InputKey::Escape: {
            CameraManager<borov_engine::SpectatorCameraManager>(camera_);
            break;
        }
        case borov_engine::InputKey::G: {
            // ReSharper disable once CppTooWideScopeInitStatement
            auto is_triangle = [](const borov_engine::Component &component) {
                return dynamic_cast<const borov_engine::TriangleComponent *>(&component) != nullptr;
            };
            for (borov_engine::Component &component : Components() | std::views::filter(is_triangle)) {
                auto &triangle_component = dynamic_cast<borov_engine::TriangleComponent &>(component);
                const bool wireframe = triangle_component.Wireframe();
                triangle_component.Wireframe(!wireframe);
            }

            // ReSharper disable once CppTooWideScopeInitStatement
            auto is_primitive = [](const borov_engine::Component &component) {
                return dynamic_cast<const borov_engine::GeometricPrimitiveComponent *>(&component) != nullptr;
            };
            for (borov_engine::Component &component : Components() | std::views::filter(is_primitive)) {
                auto &triangle_component = dynamic_cast<borov_engine::GeometricPrimitiveComponent &>(component);
                const bool wireframe = triangle_component.Wireframe();
                triangle_component.Wireframe(!wireframe);
            }
            break;
        }
        case borov_engine::InputKey::LeftButton: {
            const borov_engine::Window *window = Window();
            if (window == nullptr) {
                break;
            }

            const borov_engine::SceneComponent *target = nullptr;
            float nearest_distance = std::numeric_limits<float>::infinity();

            namespace math = borov_engine::math;
            const math::Point cursor_position = window->CursorPosition();
            const math::Vector3 world_cursor_position = ScreenToWorld(cursor_position);
            const math::Vector3 ray_position = camera_.WorldTransform().position;
            const math::Vector3 ray_direction = math::Normalize(world_cursor_position - ray_position);
            const math::Ray ray{ray_position, ray_direction};

            // ReSharper disable once CppTooWideScopeInitStatement
            auto is_scene_and_collision = [](const borov_engine::Component &component) {
                return dynamic_cast<const borov_engine::SceneComponent *>(&component) != nullptr &&
                       dynamic_cast<const borov_engine::CollisionPrimitive *>(&component) != nullptr;
            };
            for (const borov_engine::Component &component : Components() | std::views::filter(is_scene_and_collision)) {
                const auto &scene_component = dynamic_cast<const borov_engine::SceneComponent &>(component);
                const auto &collision_primitive = dynamic_cast<const borov_engine::CollisionPrimitive &>(component);
                if (float distance = 1000.0f; collision_primitive.Intersects(ray, distance)) {
                    if (target == nullptr) {
                        target = &scene_component;
                    } else if (distance < nearest_distance) {
                        target = &scene_component;
                        nearest_distance = distance;
                    }
                }
            }

            if (target != nullptr) {
                CameraManager<borov_engine::OrbitCameraManager>(camera_, *target);
            }
            break;
        }
        default: {
            break;
        }
    }
}
