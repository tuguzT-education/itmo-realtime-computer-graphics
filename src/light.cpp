#include "borov_engine/light.hpp"

#include <array>

#include "borov_engine/camera.hpp"
#include "borov_engine/projection.hpp"

#undef min
#undef max

namespace borov_engine {

LightComponent::LightComponent(class Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), light_{initializer.light}, is_enabled_{initializer.is_light_enabled} {}

bool LightComponent::IsLightEnabled() const {
    return is_enabled_;
}

bool& LightComponent::IsLightEnabled() {
    return is_enabled_;
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
        .ambient = is_enabled_ ? light_.ambient : disabled_color,
        .diffuse = is_enabled_ ? light_.diffuse : disabled_color,
        .specular = is_enabled_ ? light_.specular : disabled_color,
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

math::Frustum DirectionalLightComponent::Frustum(const Camera* camera) const {
    const math::Frustum camera_frustum = camera != nullptr ? camera->Frustum() : math::Frustum{};
    const math::Vector3 frustum_center = math::FrustumCenter(camera_frustum);

    const auto& [position, rotation, scale] = WorldTransform();
    const class Transform light_transform {
        .position = frustum_center, .rotation = rotation, .scale = scale
    };

    const math::Matrix4x4 projection = ProjectionMatrix(camera);

    math::Frustum frustum{projection, true};
    frustum.Transform(frustum, light_transform.ToMatrix());
    return frustum;
}

math::Matrix4x4 DirectionalLightComponent::ViewMatrix(const Camera* camera) const {
    const math::Frustum camera_frustum = camera != nullptr ? camera->Frustum() : math::Frustum{};
    const math::Vector3 frustum_center = math::FrustumCenter(camera_frustum);

    const auto& [position, rotation, scale] = WorldTransform();
    const class Transform light_transform {
        .position = frustum_center, .rotation = rotation, .scale = scale
    };
    return light_transform.ViewMatrix();
}

math::Matrix4x4 DirectionalLightComponent::ProjectionMatrix(const Camera* camera) const {
    const math::Frustum camera_frustum = camera != nullptr ? camera->Frustum() : math::Frustum{};
    const auto frustum_corners = math::Corners(camera_frustum);
    const math::Matrix4x4 light_view = ViewMatrix(camera);

    float left = std::numeric_limits<float>::max();
    float right = std::numeric_limits<float>::lowest();
    float bottom = std::numeric_limits<float>::max();
    float top = std::numeric_limits<float>::lowest();
    float near_plane = std::numeric_limits<float>::max();
    float far_plane = std::numeric_limits<float>::lowest();
    for (const math::Vector3& corner : frustum_corners) {
        const math::Vector3 light_view_corner = math::Vector3::Transform(corner, light_view);
        left = std::min(left, light_view_corner.x);
        right = std::max(right, light_view_corner.x);
        bottom = std::min(bottom, light_view_corner.y);
        top = std::max(top, light_view_corner.y);
        near_plane = std::min(near_plane, light_view_corner.z);
        far_plane = std::max(far_plane, light_view_corner.z);
    }

    // how much geometry to include from outside the view frustum?
    constexpr float z_mult = 1.0f;
    near_plane = (near_plane < 0) ? near_plane * z_mult : near_plane / z_mult;
    far_plane = (far_plane < 0) ? far_plane / z_mult : far_plane * z_mult;

    return math::Matrix4x4::CreateOrthographicOffCenter(left, right, bottom, top, near_plane, far_plane);
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

math::Frustum PointLightComponent::Frustum(const Camera* camera) const {
    // TODO
    return math::Frustum{};
}

math::Matrix4x4 PointLightComponent::ViewMatrix(const Camera* camera) const {
    // TODO
    return math::Matrix4x4::Identity;
}

math::Matrix4x4 PointLightComponent::ProjectionMatrix(const Camera* camera) const {
    // TODO
    return math::Matrix4x4::Identity;
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

math::Frustum SpotLightComponent::Frustum(const Camera* camera) const {
    // TODO
    return math::Frustum{};
}

math::Matrix4x4 SpotLightComponent::ViewMatrix(const Camera* camera) const {
    // TODO
    return math::Matrix4x4::Identity;
}

math::Matrix4x4 SpotLightComponent::ProjectionMatrix(const Camera* camera) const {
    // TODO
    return math::Matrix4x4::Identity;
}

}  // namespace borov_engine
