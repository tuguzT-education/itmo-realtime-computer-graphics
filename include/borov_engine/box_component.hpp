#pragma once

#ifndef BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED

#include "triangle_component.hpp"

namespace borov_engine {

class BoxComponent : public TriangleComponent {
  public:
    explicit BoxComponent(class Game &game, float length, float height, float width,
                          math::Color color = {1.0f, 1.0f, 1.0f}, const class Transform &transform = {},
                          const SceneComponent *parent = nullptr);

    [[nodiscard]] float Length() const;
    [[nodiscard]] float Height() const;
    [[nodiscard]] float Width() const;

    [[nodiscard]] math::OrientedBox Collision() const;

  private:
    float length_;
    float height_;
    float width_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
