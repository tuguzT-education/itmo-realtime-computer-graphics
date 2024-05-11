#pragma once

#ifndef BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED

#include "camera_manager.hpp"
#include "input.hpp"
#include "scene_component.hpp"

namespace borov_engine {

class OrbitCameraManager : public CameraManager {
  public:
    static const float min_distance;

    struct Initializer : CameraManager::Initializer {
        std::reference_wrapper<const SceneComponent> target;
        Camera *camera = nullptr;
        float distance = min_distance;
        float sensitivity = 1.0f;
        float zoom_speed = 1.0f;

        Initializer &Target(const SceneComponent &target);
        Initializer &Camera(Camera *camera);
        Initializer &Distance(float distance);
        Initializer &Sensitivity(float sensitivity);
        Initializer &ZoomSpeed(float zoom_speed);
    };

    explicit OrbitCameraManager(class Game &game, const Initializer &initializer);
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

    std::reference_wrapper<Camera> camera_;
    std::reference_wrapper<const SceneComponent> target_;

    float distance_;
    float sensitivity_;
    float zoom_speed_;

    math::Vector2 mouse_offset_;
    std::int32_t wheel_delta_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
