#pragma once

#ifndef BOROV_ENGINE_CONTEXT_HPP_INCLUDED
#define BOROV_ENGINE_CONTEXT_HPP_INCLUDED

#include <functional>

#include "game.hpp"

namespace borov_engine {

class Component {
  public:
    explicit Component(Game &game);
    virtual ~Component();

    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;

  protected:
    [[nodiscard]] Input *GetInput();
    [[nodiscard]] const Input *GetInput() const;

    [[nodiscard]] Window *GetWindow();
    [[nodiscard]] const Window *GetWindow() const;

    [[nodiscard]] ID3D11DeviceContext *GetDeviceContext();
    [[nodiscard]] const ID3D11DeviceContext *GetDeviceContext() const;

    [[nodiscard]] ID3D11Device *GetDevice();
    [[nodiscard]] const ID3D11Device *GetDevice() const;

    [[nodiscard]] const Color &ClearScreenColor() const;
    [[nodiscard]] Color &ClearScreenColor();

    [[nodiscard]] const Timer &Timer() const;

  private:
    std::reference_wrapper<Game> game_;
};

}

#endif //BOROV_ENGINE_CONTEXT_HPP_INCLUDED
