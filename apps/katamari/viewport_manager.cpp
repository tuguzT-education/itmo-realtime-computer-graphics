#include "viewport_manager.hpp"

#include <borov_engine/camera.hpp>

ViewportManager::ViewportManager(borov_engine::Game &game)
    : borov_engine::ViewportManager(game),
      stationary_camera_{
          Game().AddComponent<borov_engine::Camera>([] {
              namespace math = borov_engine::math;

              borov_engine::Camera::Initializer initializer{
                  .projection = std::make_unique<borov_engine::OrthographicProjection>(math::Vector2{20.0f}),
                  .far_plane = 21.0f,
              };

              initializer.transform.position = math::Vector3::Up * 20.0f;
              initializer.transform.rotation =
                  math::Quaternion::CreateFromAxisAngle(math::Vector3::Right, -std::numbers::pi_v<float> / 2.0f);
              return initializer;
          }()),
      } {}

void ViewportManager::SplitViewports(std::vector<borov_engine::Viewport> &into) {
    const borov_engine::Viewport &target_viewport = TargetViewport();

    target_viewport.camera->Width(target_viewport.width / 2.0f);
    into.emplace_back(target_viewport.x, target_viewport.y, target_viewport.width / 2.0f, target_viewport.height,
                      target_viewport.minDepth, target_viewport.maxDepth, target_viewport.camera);

    stationary_camera_.get().Width(target_viewport.width / 2.0f);
    stationary_camera_.get().Height(target_viewport.height);
    into.emplace_back(target_viewport.x + target_viewport.width / 2.0f, target_viewport.y, target_viewport.width / 2.0f,
                      target_viewport.height, target_viewport.minDepth, target_viewport.maxDepth,
                      &stationary_camera_.get());
}
