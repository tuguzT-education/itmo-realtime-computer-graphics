#pragma once

#ifndef BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED

#include "component.hpp"

namespace borov_engine {

class CameraManager : public Component {
  public:
    struct Initializer : Component::Initializer {};

    explicit CameraManager(class Game &game, const Initializer &initializer = {});

    [[nodiscard]] virtual const Camera *MainCamera() const;
    [[nodiscard]] virtual Camera *MainCamera();
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED
