#pragma once

#ifndef BOROV_ENGINE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_COMPONENT_HPP_INCLUDED

#include <d3d11.h>

#include <functional>

namespace borov_engine {

class Game;
class Camera;

class Component {
  public:
    explicit Component(Game &game);
    virtual ~Component();

    virtual void Update(float delta_time);
    virtual void Draw(const Camera *camera);
    virtual void OnTargetResize();

  protected:
    [[nodiscard]] const Game &Game() const;
    [[nodiscard]] class Game &Game();

    [[nodiscard]] ID3D11DeviceContext &DeviceContext();
    [[nodiscard]] const ID3D11DeviceContext &DeviceContext() const;

    [[nodiscard]] ID3D11Device &Device();
    [[nodiscard]] const ID3D11Device &Device() const;

  private:
    std::reference_wrapper<class Game> game_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_COMPONENT_HPP_INCLUDED
