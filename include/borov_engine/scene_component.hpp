#pragma once

#ifndef BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "concepts.hpp"
#include "transform.hpp"

namespace borov_engine {

template <typename Range>
concept SceneComponentRange = RefWrapperRange<Range, const class SceneComponent>;

class SceneComponent : public Component {
  public:
    explicit SceneComponent(class Game &game, const Transform &transform = {}, const SceneComponent *parent = nullptr);

    [[nodiscard]] const Transform &Transform() const;
    [[nodiscard]] class Transform &Transform();

    [[nodiscard]] class Transform WorldTransform() const;
    void WorldTransform(const class Transform &world_transform);

    [[nodiscard]] const SceneComponent *Parent() const;
    bool Parent(const SceneComponent *parent);

    [[nodiscard]] SceneComponentRange auto Parents() const;
    [[nodiscard]] SceneComponentRange auto Children() const;

  private:
    class Transform transform_;
    const SceneComponent *parent_;
};

}  // namespace borov_engine

#include "scene_component.inl"

#endif  // BOROV_ENGINE_SCENE_COMPONENT_HPP_INCLUDED
