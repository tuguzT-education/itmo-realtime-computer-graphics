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

auto SpotLightComponent::Initializer::Color(const math::Color color) -> Initializer& {
    this->color = color;
    return *this;
}

auto SpotLightComponent::Initializer::Attenuation(const borov_engine::Attenuation& attenuation) -> Initializer& {
    this->attenuation = attenuation;
    return *this;
}

auto SpotLightComponent::Initializer::InnerConeAngle(const float inner_cone_angle) -> Initializer& {
    this->inner_cone_angle = inner_cone_angle;
    return *this;
}

auto SpotLightComponent::Initializer::OuterConeAngle(const float outer_cone_angle) -> Initializer& {
    this->outer_cone_angle = outer_cone_angle;
    return *this;
}

SpotLightComponent::SpotLightComponent(class Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      color_{initializer.color},
      attenuation_{initializer.attenuation},
      inner_cone_angle_{initializer.inner_cone_angle},
      outer_cone_angle_{initializer.outer_cone_angle} {}

auto SpotLightComponent::Primitive() const -> PrimitiveType {
    const class Transform world_transform = WorldTransform();
    return SpotLight{
        .position = world_transform.position,
        .direction = world_transform.Forward(),
        .color = color_,
        .attenuation = attenuation_,
        .inner_cone_angle = inner_cone_angle_,
        .outer_cone_angle = outer_cone_angle_,
    };
}

math::Vector3 SpotLightComponent::Direction() const {
    return WorldTransform().Forward();
}

math::Color SpotLightComponent::Color() const {
    return color_;
}

math::Color& SpotLightComponent::Color() {
    return color_;
}

const Attenuation& SpotLightComponent::Attenuation() const {
    return attenuation_;
}

Attenuation& SpotLightComponent::Attenuation() {
    return attenuation_;
}

float SpotLightComponent::InnerConeAngle() const {
    return inner_cone_angle_;
}

float& SpotLightComponent::InnerConeAngle() {
    return inner_cone_angle_;
}

float SpotLightComponent::OuterConeAngle() const {
    return outer_cone_angle_;
}

float& SpotLightComponent::OuterConeAngle() {
    return outer_cone_angle_;
}

}  // namespace borov_engine
