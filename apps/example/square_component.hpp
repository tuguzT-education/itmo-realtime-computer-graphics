#pragma once

#ifndef APPS_SQUARE_COMPONENT_HPP_INCLUDED
#define APPS_SQUARE_COMPONENT_HPP_INCLUDED

#include <borov_engine/triangle_component.hpp>

class SquareComponent : public borov_engine::TriangleComponent {
  public:
    explicit SquareComponent(borov_engine::Game &game);

    void Update(float delta_time) override;
};

#endif //APPS_SQUARE_COMPONENT_HPP_INCLUDED
