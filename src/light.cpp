#include "borov_engine/light.hpp"

namespace borov_engine {

AmbientLightComponent::AmbientLightComponent(class Game& game, const Initializer& initializer)
    : Component(game, initializer), light_{initializer.light} {}

const AmbientLightComponent::PrimitiveType& AmbientLightComponent::Primitive() const noexcept {
    return light_;
}

AmbientLightComponent::PrimitiveType& AmbientLightComponent::Primitive() noexcept {
    return light_;
}

DirectionalLightComponent::DirectionalLightComponent(class Game& game, const Initializer& initializer)
    : Component(game, initializer), light_{initializer.light} {}

const DirectionalLightComponent::PrimitiveType& DirectionalLightComponent::Primitive() const noexcept {
    return light_;
}

DirectionalLightComponent::PrimitiveType& DirectionalLightComponent::Primitive() noexcept {
    return light_;
}

auto PointLightComponent::Initializer::Color(const math::Color color) -> Initializer& {
    this->color = color;
    return *this;
}

auto PointLightComponent::Initializer::Attenuation(const class Attenuation& attenuation) -> Initializer& {
    this->attenuation = attenuation;
    return *this;
}

PointLightComponent::PointLightComponent(class Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), color_{initializer.color}, attenuation_{initializer.attenuation} {}

PointLightComponent::PrimitiveType PointLightComponent::Primitive() const {
    const auto [position, rotation, scale] = WorldTransform();
    const class Attenuation attenuation {
        .const_factor = attenuation_.const_factor * math::Color{scale},
        .linear_factor = attenuation_.linear_factor * math::Color{scale},
        .quad_factor = attenuation_.quad_factor * math::Color{scale},
    };
    return PointLight{
        .position = position,
        .color = color_,
        .attenuation = attenuation,
    };
}

math::Color PointLightComponent::Color() const {
    return color_;
}

math::Color& PointLightComponent::Color() {
    return color_;
}

const Attenuation& PointLightComponent::Attenuation() const {
    return attenuation_;
}

Attenuation& PointLightComponent::Attenuation() {
    return attenuation_;
}

}  // namespace borov_engine
