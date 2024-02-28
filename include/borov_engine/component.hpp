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
    [[nodiscard]] borov_engine::Input *Input();
    [[nodiscard]] const borov_engine::Input *Input() const;

    [[nodiscard]] borov_engine::Window *Window();
    [[nodiscard]] const borov_engine::Window *Window() const;

    [[nodiscard]] ID3D11DeviceContext *DeviceContext();
    [[nodiscard]] const ID3D11DeviceContext *DeviceContext() const;

    [[nodiscard]] ID3D11Device *Device();
    [[nodiscard]] const ID3D11Device *Device() const;

    [[nodiscard]] const Color &ClearScreenColor() const;
    [[nodiscard]] Color &ClearScreenColor();

    [[nodiscard]] const Timer &Timer() const;

  private:
    std::reference_wrapper<Game> game_;
};

}

#endif //BOROV_ENGINE_CONTEXT_HPP_INCLUDED
