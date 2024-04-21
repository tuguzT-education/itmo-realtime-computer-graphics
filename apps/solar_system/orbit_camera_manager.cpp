#include "orbit_camera_manager.hpp"

#include <borov_engine/game.hpp>

OrbitCameraManager::OrbitCameraManager(borov_engine::Game& game, borov_engine::Camera& camera)
    : CameraManager(game), camera_{camera} {
    camera.ProjectionType() = borov_engine::PerspectiveCameraProjectionType{};
}

const borov_engine::Camera* OrbitCameraManager::MainCamera() const {
    return &camera_.get();
}

borov_engine::Camera* OrbitCameraManager::MainCamera() {
    return &camera_.get();
}

void OrbitCameraManager::Update(const float delta_time) {
    if (const auto input = Game().Input(); input != nullptr) {
        namespace math = borov_engine::math;

        borov_engine::Transform& transform = camera_.get().Transform();
        // TODO
    }
}
