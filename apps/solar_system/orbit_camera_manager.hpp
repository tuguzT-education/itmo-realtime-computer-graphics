#pragma once

#ifndef SOLAR_SYSTEM_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
#define SOLAR_SYSTEM_ORBIT_CAMERA_MANAGER_HPP_INCLUDED

#include <borov_engine/camera_manager.hpp>

class OrbitCameraManager final : public borov_engine::CameraManager {
  public:
    explicit OrbitCameraManager(borov_engine::Game &game, borov_engine::Camera &camera);

    [[nodiscard]] const borov_engine::Camera *MainCamera() const override;
    [[nodiscard]] borov_engine::Camera *MainCamera() override;

    void Update(float delta_time) override;

  private:
    std::reference_wrapper<borov_engine::Camera> camera_;
};

#endif  // SOLAR_SYSTEM_ORBIT_CAMERA_MANAGER_HPP_INCLUDED
