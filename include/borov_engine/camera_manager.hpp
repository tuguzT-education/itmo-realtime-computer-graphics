#pragma once

#ifndef BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED

#include "component.hpp"

namespace borov_engine {

class CameraManager : public Component {
  public:
    explicit CameraManager(borov_engine::Game &game);

    [[nodiscard]] virtual const Camera *MainCamera() const;
    [[nodiscard]] virtual Camera *MainCamera();
};

}

#endif //BOROV_ENGINE_CAMERA_MANAGER_HPP_INCLUDED
