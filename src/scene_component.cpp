#include "borov_engine/scene_component.hpp"

namespace borov_engine {

auto SceneComponent::Initializer::Transform(const borov_engine::Transform &transform) -> Initializer & {
    this->transform = transform;
    return *this;
}

auto SceneComponent::Initializer::Parent(const SceneComponent *parent) -> Initializer & {
    this->parent = parent;
    return *this;
}

SceneComponent::SceneComponent(class Game &game, const Initializer &initializer)
    : Component(game), transform_{initializer.transform}, parent_{initializer.parent} {}

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

bool SceneComponent::Parent(const SceneComponent *parent) {
    if (parent == this) {
        return false;
    }

    const class Transform world_transform = WorldTransform();
    parent_ = parent;
    WorldTransform(world_transform);
    return true;
}

bool SceneComponent::IsParentOf(const SceneComponent &scene_component, const std::size_t max_depth) const {
    return scene_component.IsChildOf(*this, max_depth);
}

bool SceneComponent::IsChildOf(const SceneComponent &scene_component, std::size_t max_depth) const {
    const SceneComponent *parent = Parent();
    while (parent != nullptr && max_depth > 0) {
        if (&scene_component == parent) {
            return true;
        }
        parent = parent->Parent();
        max_depth -= 1;
    }
    return false;
}

}  // namespace borov_engine
