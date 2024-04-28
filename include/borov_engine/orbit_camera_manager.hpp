#pragma once

#ifndef BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED

#include "camera_manager.hpp"
#include "input.hpp"
#include "scene_component.hpp"

namespace borov_engine {

class OrbitCameraManager : public CameraManager {
  public:
    explicit OrbitCameraManager(class Game &game, const SceneComponent &target, float distance = 0.0f,
                                float sensitivity = 1.0f, float zoom_speed = 1.0f);
    explicit OrbitCameraManager(class Game &game, Camera &camera, const SceneComponent &target, float distance = 0.0f,
                                float sensitivity = 1.0f, float zoom_speed = 1.0f);
    ~OrbitCameraManager() override;

    [[nodiscard]] float Distance() const;
    bool Distance(float distance);

    [[nodiscard]] float Sensitivity() const;
    bool Sensitivity(float sensitivity);

    [[nodiscard]] float ZoomSpeed() const;
    bool ZoomSpeed(float zoom_speed);

    [[nodiscard]] const Camera *MainCamera() const override;
    [[nodiscard]] Camera *MainCamera() override;

    void Update(float delta_time) override;

  private:
    void OnMouseMove(const MouseMoveData &data);

    math::Vector2 mouse_offset_;
    std::int32_t wheel_delta_;

    float distance_;
    float sensitivity_;
    float zoom_speed_;

    std::reference_wrapper<Camera> camera_;
    std::reference_wrapper<const SceneComponent> target_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
