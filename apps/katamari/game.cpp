#include "game.hpp"

#include "borov_engine/camera.hpp"
#include "borov_engine/orbit_camera_manager.hpp"

Game::Game(borov_engine::Window& window, borov_engine::Input& input)
    : borov_engine::Game(window, input),
      camera_{
          AddComponent<borov_engine::Camera>([] {
              borov_engine::Camera::Initializer initializer{
                  .projection_type = borov_engine::PerspectiveCameraProjectionType{},
              };
              initializer.Transform({.position = borov_engine::math::Vector3::Backward});
              return initializer;
          }()),
      },
      field_{AddComponent<Field>()},
      player_{AddComponent<Player>()} {
    CameraManager<borov_engine::OrbitCameraManager>(borov_engine::OrbitCameraManager::Initializer{
        .target = player_,
        .camera = &camera_.get(),
    });
}

void Game::Update(const float delta_time) {
    borov_engine::Game::Update(delta_time);

    namespace math = borov_engine::math;

    const borov_engine::Input* input = Input();
    if (input == nullptr) {
        return;
    }

    const borov_engine::Transform camera_transform = camera_.get().WorldTransform();

    auto [left, right, forward, backward] = player_.get().Controls();
    const auto x = static_cast<float>(input->IsKeyDown(right) - input->IsKeyDown(left));
    const auto z = static_cast<float>(input->IsKeyDown(forward) - input->IsKeyDown(backward));

    math::Vector3 direction = camera_transform.Right() * x + camera_transform.Forward() * z;
    direction.y = 0.0f;
    direction.Normalize();

    borov_engine::Transform player_transform = player_.get().WorldTransform();
    player_transform.position += direction * delta_time;
    if (direction != math::Vector3::Zero) {
        const auto additional =
            math::Quaternion::CreateFromAxisAngle(-direction.Cross(math::Vector3::Up), 1.0f * delta_time);
        player_transform.rotation = math::Quaternion::Concatenate(additional, player_transform.rotation);
    }
    player_.get().WorldTransform(player_transform);
}
