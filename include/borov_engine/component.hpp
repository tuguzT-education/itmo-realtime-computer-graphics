#pragma once

#ifndef BOROV_ENGINE_CONTEXT_HPP_INCLUDED
#define BOROV_ENGINE_CONTEXT_HPP_INCLUDED

#include <d3d11.h>

#include <functional>

#include "game.fwd"

namespace borov_engine {

class Component {
  public:
    explicit Component(Game &game);
    virtual ~Component() = default;

    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;

  protected:
    ID3D11DeviceContext *GetDeviceContext();
    ID3D11Device *GetDevice();

  private:
    std::reference_wrapper<Game> game_;
};

}

#endif //BOROV_ENGINE_CONTEXT_HPP_INCLUDED
