#pragma once

#ifndef BOROV_ENGINE_VIEWPORT_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_VIEWPORT_MANAGER_HPP_INCLUDED

#include <span>

#include "component.hpp"
#include "viewport.hpp"

namespace borov_engine {

class ViewportManager : public Component {
  public:
    struct Initializer : Component::Initializer {};

    explicit ViewportManager(class Game &game, const Initializer &initializer = {});

    [[nodiscard]] const Viewport &TargetViewport() const;

    [[nodiscard]] std::span<const Viewport> Viewports() const;

    void Update(float delta_time) override;
    void OnTargetResize() override;

  protected:
    virtual void SplitViewports(std::vector<Viewport> &into);

  private:
    Viewport target_viewport_;
    std::vector<Viewport> split_viewports_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_VIEWPORT_MANAGER_HPP_INCLUDED
