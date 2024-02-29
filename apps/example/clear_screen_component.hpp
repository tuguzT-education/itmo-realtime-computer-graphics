#pragma once

#ifndef APPS_CLEAR_SCREEN_COMPONENT_HPP_INCLUDED
#define APPS_CLEAR_SCREEN_COMPONENT_HPP_INCLUDED

#include <borov_engine/component.hpp>

class ClearScreenComponent : public borov_engine::Component {
  public:
    explicit ClearScreenComponent(borov_engine::Game &game);

    void Update(float delta_time) override;
    void Draw() override;
};

ClearScreenComponent::ClearScreenComponent(borov_engine::Game &game) : Component(game) {}

void ClearScreenComponent::Update(float delta_time) {}

void ClearScreenComponent::Draw() {
    float start_time = Timer().StartTime();
    float red = start_time - std::floor(start_time);
    ClearColor() = borov_engine::math::Color{red, 0.1f, 0.1f, 1.0f};
}

#endif //APPS_CLEAR_SCREEN_COMPONENT_HPP_INCLUDED
