#include "borov_engine/scene_component.hpp"

namespace borov_engine {

SceneComponent::SceneComponent(class Game &game, const class Transform &transform, const SceneComponent *parent)
    : Component(game), transform_{transform}, parent_{parent} {}

const Transform &SceneComponent::Transform() const {
    return transform_;
}

Transform &SceneComponent::Transform() {
    return transform_;
}

Transform SceneComponent::WorldTransform() const {
    class Transform world_current = transform_;
    if (parent_ != nullptr) {
        const class Transform world_parent = parent_->WorldTransform();
        world_current = Transform::Concatenate(world_parent, world_current);
    }

    return world_current;
}

void SceneComponent::WorldTransform(const class Transform &world_transform) {
    if (parent_ == nullptr) {
        transform_ = world_transform;
        return;
    }

    const class Transform inv_world_parent = Transform::Inverse(parent_->WorldTransform());
    transform_ = Transform::Concatenate(inv_world_parent, world_transform);
}

const SceneComponent *SceneComponent::Parent() const {
    return parent_;
}

void SceneComponent::Parent(const SceneComponent *parent) {
    if (parent == this) {
        return;
    }
    parent_ = parent;
}

}  // namespace borov_engine
