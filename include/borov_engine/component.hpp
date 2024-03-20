#pragma once

#ifndef BOROV_ENGINE_CONTEXT_HPP_INCLUDED
#define BOROV_ENGINE_CONTEXT_HPP_INCLUDED

#include <d3d11.h>

#include <functional>

#include "camera.hpp"

namespace borov_engine {

class Game;

class Component {
  public:
    explicit Component(borov_engine::Game &game);
    virtual ~Component();

    virtual void Update(float delta_time);
    virtual void Draw(const Camera *camera);
    virtual void OnTargetResize();

  protected:
    [[nodiscard]] borov_engine::Game &Game();
    [[nodiscard]] const borov_engine::Game &Game() const;

    [[nodiscard]] ID3D11DeviceContext &DeviceContext();
    [[nodiscard]] const ID3D11DeviceContext &DeviceContext() const;

    [[nodiscard]] ID3D11Device &Device();
    [[nodiscard]] const ID3D11Device &Device() const;

  private:
    std::reference_wrapper<borov_engine::Game> game_;
};

}

#endif //BOROV_ENGINE_CONTEXT_HPP_INCLUDED
