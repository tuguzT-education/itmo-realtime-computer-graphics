#include "borov_engine/scene_component.hpp"

namespace borov_engine {

SceneComponent::SceneComponent(borov_engine::Game &game, const borov_engine::Transform &transform)
    : Component(game), transform_{transform} {}

const Transform &SceneComponent::Transform() const {
    return transform_;
}

Transform &SceneComponent::Transform() {
    return transform_;
}

}  // namespace borov_engine
