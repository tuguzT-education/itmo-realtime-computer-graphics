#pragma once

#ifndef SOLAR_SYSTEM_CAMERA_MANAGER_HPP_INCLUDED
#define SOLAR_SYSTEM_CAMERA_MANAGER_HPP_INCLUDED

#include <borov_engine/camera_manager.hpp>
#include <borov_engine/input.hpp>

class CameraManager final : public borov_engine::CameraManager {
  public:
    explicit CameraManager(borov_engine::Game &game, float yaw = 0.0f, float pitch = 0.0f);
    ~CameraManager() override;

    [[nodiscard]] const borov_engine::Camera *MainCamera() const override;
    [[nodiscard]] borov_engine::Camera *MainCamera() override;

    [[nodiscard]] float Yaw() const;
    void Yaw(float yaw);

    [[nodiscard]] float Pitch() const;
    void Pitch(float pitch);

    void Update(float delta_time) override;

  private:
    void OnMouseMove(borov_engine::MouseMoveData data);

    borov_engine::math::Vector2 mouse_offset_;
    std::int32_t wheel_delta_;

    borov_engine::Camera camera_;
    float yaw_;
    float pitch_;
};

#endif  // SOLAR_SYSTEM_CAMERA_MANAGER_HPP_INCLUDED
