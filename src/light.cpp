#include "borov_engine/light.hpp"

namespace borov_engine {

LightComponent::LightComponent(class Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), light_{initializer.light}, enabled_{initializer.light_enabled} {}

bool LightComponent::LightEnabled() const {
    return enabled_;
}

bool& LightComponent::LightEnabled() {
    return enabled_;
}

math::Color LightComponent::Ambient() const {
    return light_.ambient;
}

math::Color& LightComponent::Ambient() {
    return light_.ambient;
}

math::Color LightComponent::Diffuse() const {
    return light_.diffuse;
}

math::Color& LightComponent::Diffuse() {
    return light_.diffuse;
}

math::Color LightComponent::Specular() const {
    return light_.specular;
}

math::Color& LightComponent::Specular() {
    return light_.specular;
}

Light LightComponent::Light() const {
    const math::Color disabled_color{math::colors::linear::Black};
    return {
        .ambient = enabled_ ? light_.ambient : disabled_color,
        .diffuse = enabled_ ? light_.diffuse : disabled_color,
        .specular = enabled_ ? light_.specular : disabled_color,
    };
}

math::Vector3 DirectionalLightComponent::Initializer::Direction() const {
    return transform.Forward();
}

void DirectionalLightComponent::Initializer::Direction(const math::Vector3& direction) {
    transform.rotation = math::Quaternion::LookRotation(direction, math::Vector3::Zero);
}

DirectionalLightComponent::DirectionalLightComponent(class Game& game, const Initializer& initializer)
    : LightComponent(game, initializer) {}

math::Vector3 DirectionalLightComponent::Direction() const {
    return Transform().Forward();
}

void DirectionalLightComponent::Direction(const math::Vector3& direction) {
    Transform().rotation = math::Quaternion::LookRotation(direction, math::Vector3::Zero);
}

DirectionalLight DirectionalLightComponent::DirectionalLight() const {
    const auto [ambient, diffuse, specular] = Light();

    class DirectionalLight directional_light = {
        .direction = Direction(),
    };
    directional_light.ambient = ambient;
    directional_light.diffuse = diffuse;
    directional_light.specular = specular;
    return directional_light;
}

PointLightComponent::PointLightComponent(class Game& game, const Initializer& initializer)
    : LightComponent(game, initializer), attenuation_{initializer.attenuation} {}

const Attenuation& PointLightComponent::Attenuation() const {
    return attenuation_;
}

Attenuation& PointLightComponent::Attenuation() {
    return attenuation_;
}

PointLight PointLightComponent::PointLight() const {
    const auto [ambient, diffuse, specular] = Light();
    const auto [position, rotation, scale] = WorldTransform();
    const class Attenuation attenuation {
        .const_factor = attenuation_.const_factor * math::Color{scale},
        .linear_factor = attenuation_.linear_factor * math::Color{scale},
        .quad_factor = attenuation_.quad_factor * math::Color{scale},
    };

    class PointLight point_light = {
        .position = position,
        .attenuation = attenuation,
    };
    point_light.ambient = ambient;
    point_light.diffuse = diffuse;
    point_light.specular = specular;
    return point_light;
}

math::Vector3 SpotLightComponent::Initializer::Direction() const {
    return transform.Forward();
}

void SpotLightComponent::Initializer::Direction(const math::Vector3& direction) {
    transform.rotation = math::Quaternion::LookRotation(direction, math::Vector3::Zero);
}

SpotLightComponent::SpotLightComponent(class Game& game, const Initializer& initializer)
    : LightComponent(game, initializer),
      attenuation_{initializer.attenuation},
      inner_cone_angle_{initializer.inner_cone_angle},
      outer_cone_angle_{initializer.outer_cone_angle} {}

math::Vector3 SpotLightComponent::Direction() const {
    return Transform().Forward();
}

void SpotLightComponent::Direction(const math::Vector3& direction) {
    Transform().rotation = math::Quaternion::LookRotation(direction, math::Vector3::Zero);
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

SpotLight SpotLightComponent::SpotLight() const {
    const auto [ambient, diffuse, specular] = Light();
    const auto [position, rotation, scale] = WorldTransform();
    const class Attenuation attenuation {
        .const_factor = attenuation_.const_factor * math::Color{scale},
        .linear_factor = attenuation_.linear_factor * math::Color{scale},
        .quad_factor = attenuation_.quad_factor * math::Color{scale},
    };

    class SpotLight spot_light = {
        .direction = Direction(),
        .position = position,
        .attenuation = attenuation,
        .inner_cone_angle = inner_cone_angle_,
        .outer_cone_angle = outer_cone_angle_,
    };
    spot_light.ambient = ambient;
    spot_light.diffuse = diffuse;
    spot_light.specular = specular;
    return spot_light;
}

}  // namespace borov_engine
