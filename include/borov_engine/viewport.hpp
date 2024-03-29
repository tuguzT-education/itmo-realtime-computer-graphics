#pragma once

#ifndef BOROV_ENGINE_VIEWPORT_HPP_INCLUDED
#define BOROV_ENGINE_VIEWPORT_HPP_INCLUDED

#include "camera.hpp"

namespace borov_engine {

class Viewport : public math::Viewport {
  public:
    Camera *camera;

    explicit Viewport(Camera *camera = nullptr) noexcept;

    Viewport(float x,
             float y,
             float width,
             float height,
             float min_depth = 0.0f,
             float max_depth = 1.0f,
             Camera *camera = nullptr) noexcept;

    explicit Viewport(const RECT &rect, Camera *camera = nullptr) noexcept;
};

}

#endif //BOROV_ENGINE_VIEWPORT_HPP_INCLUDED
