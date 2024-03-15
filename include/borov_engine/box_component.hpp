#pragma once

#ifndef BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED

#include <DirectXCollision.h>

#include "triangle_component.hpp"

namespace borov_engine {

class BoxComponent : public TriangleComponent {
  public:
    using Box = DirectX::BoundingOrientedBox;

    explicit BoxComponent(borov_engine::Game &game,
                          float length, float height, float width,
                          math::Color color = {1.0f, 1.0f, 1.0f},
                          borov_engine::Transform transform = {});

    [[nodiscard]] float Length() const;
    [[nodiscard]] float Height() const;
    [[nodiscard]] float Width() const;

    [[nodiscard]] Box Collision() const;

  private:
    float length_;
    float height_;
    float width_;
};

}

#endif //BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
