#pragma once

#ifndef BOROV_ENGINE_SCENE_COMPONENT_INL_INCLUDED
#define BOROV_ENGINE_SCENE_COMPONENT_INL_INCLUDED

#include "game.hpp"

namespace borov_engine {

namespace detail {

inline SceneComponentRange auto SceneComponents(const Game &game) {
    auto is_scene = [](const Component &component) {
        const auto as_scene_component = dynamic_cast<const SceneComponent *>(&component);
        return as_scene_component != nullptr;
    };
    auto to_scene = [](const Component &component) {
        const auto &scene_component = dynamic_cast<const SceneComponent &>(component);
        return std::cref(scene_component);
    };
    return game.Components() | std::views::filter(is_scene) | std::views::transform(to_scene);
}

}  // namespace detail

inline SceneComponentRange auto SceneComponent::Parents() const {
    auto is_child_of = [this](const SceneComponent &scene_component) { return IsChildOf(scene_component); };
    return detail::SceneComponents(Game()) | std::views::filter(is_child_of);
}

inline SceneComponentRange auto SceneComponent::Children() const {
    auto is_parent_of = [this](const SceneComponent &scene_component) { return IsParentOf(scene_component); };
    return detail::SceneComponents(Game()) | std::views::filter(is_parent_of);
}

}  // namespace borov_engine

#endif  // BOROV_ENGINE_SCENE_COMPONENT_INL_INCLUDED
