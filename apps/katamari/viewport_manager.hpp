#pragma once

#ifndef KATAMARI_VIEWPORT_MANAGER_HPP_INCLUDED
#define KATAMARI_VIEWPORT_MANAGER_HPP_INCLUDED

#include <borov_engine/viewport_manager.hpp>

class ViewportManager final : public borov_engine::ViewportManager {
  public:
    explicit ViewportManager(borov_engine::Game &game);

  protected:
    void SplitViewports(std::vector<borov_engine::Viewport> &into) override;

  private:
    std::reference_wrapper<borov_engine::Camera> stationary_camera_;
};

#endif  // KATAMARI_VIEWPORT_MANAGER_HPP_INCLUDED
