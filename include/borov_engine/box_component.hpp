#pragma once

#ifndef BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED

#include <DirectXCollision.h>

#include "triangle_component.hpp"

namespace borov_engine {

class BoxComponent : public TriangleComponent {
  public:
    using Box = DirectX::BoundingBox;

    explicit BoxComponent(Game &game, float width, float height, math::Color color, math::Vector3 position = {});

    [[nodiscard]] float Width() const;
    [[nodiscard]] float Height() const;
    [[nodiscard]] Box Collision() const;

  private:
    float width_;
    float height_;
};

}

#endif //BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
