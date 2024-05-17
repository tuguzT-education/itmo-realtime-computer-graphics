#include "borov_engine/light.hpp"

namespace borov_engine {

DirectionalLightComponent::DirectionalLightComponent(borov_engine::Game& game, const Initializer& initializer)
    : Component(game, initializer), light_{initializer.light} {}

const DirectionalLightComponent::PrimitiveType& DirectionalLightComponent::Primitive() const noexcept {
    return light_;
}

DirectionalLightComponent::PrimitiveType& DirectionalLightComponent::Primitive() noexcept {
    return light_;
}

}  // namespace borov_engine
