#include "game.hpp"

#include "borov_engine/camera.hpp"
#include "borov_engine/orbit_camera_manager.hpp"

Game::Game(borov_engine::Window& window, borov_engine::Input& input)
    : borov_engine::Game(window, input),
      camera_{
          AddComponent<borov_engine::Camera>(reinterpret_cast<borov_engine::Camera::Initializer&>(
              borov_engine::Camera::Initializer{.projection_type = borov_engine::PerspectiveCameraProjectionType{}}
                  .Transform(borov_engine::Transform{.position = borov_engine::math::Vector3::Backward}))),
      },
      field_{AddComponent<Field>()},
      player_{AddComponent<Player>()} {
    CameraManager<borov_engine::OrbitCameraManager>(borov_engine::OrbitCameraManager::Initializer{
        .target = player_,
        .camera = &camera_.get(),
    });
}
