#pragma once

#ifndef BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "transform.hpp"

namespace borov_engine {

class SceneComponent : public Component {
  public:
    explicit SceneComponent(class Game &game, const Transform &transform = {});

    [[nodiscard]] const Transform &Transform() const;
    [[nodiscard]] class Transform &Transform();

  private:
    class Transform transform_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED
