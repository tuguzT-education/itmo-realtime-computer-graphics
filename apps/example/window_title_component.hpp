#pragma once

#ifndef APPS_WINDOW_TEXT_COMPONENT_HPP_INCLUDED
#define APPS_WINDOW_TEXT_COMPONENT_HPP_INCLUDED

#include "borov_engine/component.hpp"

class WindowTitleComponent : public borov_engine::Component {
  public:
    explicit WindowTitleComponent(borov_engine::Game &game);

    void Update(float delta_time) override;
    void Draw() override;

  private:
    std::string initial_title_;
};

WindowTitleComponent::WindowTitleComponent(borov_engine::Game &game) : Component(game) {}

void WindowTitleComponent::Update(float delta_time) {}

void WindowTitleComponent::Draw() {
    borov_engine::Window *window = Window();
    if (window == nullptr) {
        return;
    }

    float fps = Timer().FramesPerSecond();
    if (fps <= 0) {
        initial_title_ = window->Title();
        return;
    }

    std::string title = initial_title_;
    std::format_to(std::back_inserter(title), " (FPS: {})", fps);
    window->Title(title);
}

#endif //APPS_WINDOW_TEXT_COMPONENT_HPP_INCLUDED
