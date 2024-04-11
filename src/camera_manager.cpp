#include "borov_engine/camera_manager.hpp"

namespace borov_engine {

CameraManager::CameraManager(borov_engine::Game &game) : Component(game) {}

const Camera *CameraManager::MainCamera() const {
    return nullptr;
}

Camera *CameraManager::MainCamera() {
    return nullptr;
}

}  // namespace borov_engine
