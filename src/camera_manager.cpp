#include "borov_engine/camera_manager.hpp"

namespace borov_engine {

CameraManager::CameraManager(class Game &game, const Initializer &initializer) : Component(game, initializer) {}

const Camera *CameraManager::MainCamera() const {
    return nullptr;
}

Camera *CameraManager::MainCamera() {
    return nullptr;
}

}  // namespace borov_engine
