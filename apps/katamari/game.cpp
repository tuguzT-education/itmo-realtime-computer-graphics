#include "game.hpp"

#include "borov_engine/camera.hpp"
#include "borov_engine/orbit_camera_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
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
      player_{AddComponent<Player>()},
      hog_{AddComponent<Hog>()} {
    CameraManager<borov_engine::OrbitCameraManager>(borov_engine::OrbitCameraManager::Initializer{
        .target = player_,
        .camera = &camera_.get(),
    });
}

void Game::Update(const float delta_time) {
    borov_engine::Game::Update(delta_time);

    namespace math = borov_engine::math;

    const borov_engine::Input *input = Input();
    if (input == nullptr) {
        return;
    }

    const borov_engine::Transform camera_transform = camera_.get().WorldTransform();

    auto [left_key, right_key, forward_key, backward_key] = player_.get().Controls();
    const auto x = static_cast<float>(input->IsKeyDown(right_key) - input->IsKeyDown(left_key));
    const auto z = static_cast<float>(input->IsKeyDown(forward_key) - input->IsKeyDown(backward_key));

    const math::Vector3 right = camera_transform.Right();
    math::Vector3 forward = camera_transform.Forward();
    forward.y = 0.0f;
    forward.Normalize();
    if (forward.LengthSquared() <= 0.1f) {
        forward = right.Cross(math::Vector3::Up);
    }

    math::Vector3 direction = right * x + forward * z;
    direction.y = 0.0f;
    direction.Normalize();

    borov_engine::Transform player_transform = player_.get().WorldTransform();
    player_transform.position += direction * delta_time;
    if (direction != math::Vector3::Zero) {
        const auto additional = math::Quaternion::CreateFromAxisAngle(-direction.Cross(math::Vector3::Up),
                                                                      std::numbers::pi_v<float> * 0.5f * delta_time);
        player_transform.rotation = math::Quaternion::Concatenate(additional, player_transform.rotation);
    }
    player_.get().WorldTransform(player_transform);
}
