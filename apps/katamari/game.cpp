#include "game.hpp"

#include "borov_engine/camera.hpp"
#include "borov_engine/orbit_camera_manager.hpp"

Game::Game(borov_engine::Window& window, borov_engine::Input& input)
    : borov_engine::Game(window, input),
      camera_{
          AddComponent<borov_engine::Camera>(
              borov_engine::PerspectiveCameraProjectionType{},
              borov_engine::Transform{.position = borov_engine::math::Vector3::Backward}),
      },
      player_{AddComponent<Player>()} {
    CameraManager<borov_engine::OrbitCameraManager>(camera_, player_);
}
