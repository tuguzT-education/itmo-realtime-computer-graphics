#pragma once

#ifndef SOLAR_SYSTEM_VIEWPORT_MANAGER_HPP_INCLUDED
#define SOLAR_SYSTEM_VIEWPORT_MANAGER_HPP_INCLUDED

#include <borov_engine/viewport_manager.hpp>

class ViewportManager : public borov_engine::ViewportManager {
  public:
    explicit ViewportManager(borov_engine::Game &game);

  protected:
    void SplitViewports(std::vector<borov_engine::Viewport> &into) override;
};

#endif //SOLAR_SYSTEM_VIEWPORT_MANAGER_HPP_INCLUDED
