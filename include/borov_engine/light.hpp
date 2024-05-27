#pragma once

#ifndef BOROV_ENGINE_LIGHT_HPP_INCLUDED
#define BOROV_ENGINE_LIGHT_HPP_INCLUDED

#include "scene_component.hpp"

namespace borov_engine {

struct alignas(16) Light {
    math::Color ambient{math::colors::linear::White};
    math::Color diffuse;
    math::Color specular;
};

class LightComponent : public SceneComponent {
  public:
    struct Initializer : SceneComponent::Initializer {
        Light light;
        bool light_enabled = false;
    };

    explicit LightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] math::Color Ambient() const;
    [[nodiscard]] math::Color& Ambient();

    [[nodiscard]] math::Color Diffuse() const;
    [[nodiscard]] math::Color& Diffuse();

    [[nodiscard]] math::Color Specular() const;
    [[nodiscard]] math::Color& Specular();

    [[nodiscard]] Light Light() const;

    [[nodiscard]] bool LightEnabled() const;
    [[nodiscard]] bool& LightEnabled();

    [[nodiscard]] math::Matrix4x4 ViewMatrix() const;
    [[nodiscard]] virtual math::Matrix4x4 ProjectionMatrix() const = 0;

  private:
    class Light light_;
    bool enabled_;
};

struct alignas(16) DirectionalLight : Light {
    alignas(16) math::Vector3 direction;
};

class DirectionalLightComponent : public LightComponent {
  public:
    struct Initializer : LightComponent::Initializer {
        [[nodiscard]] math::Vector3 Direction() const;
        void Direction(const math::Vector3& direction);
    };

    explicit DirectionalLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] math::Vector3 Direction() const;
    void Direction(const math::Vector3& direction);

    [[nodiscard]] DirectionalLight DirectionalLight() const;

    [[nodiscard]] math::Matrix4x4 ProjectionMatrix() const override;
};

struct alignas(16) Attenuation {
    math::Color const_factor{math::colors::linear::White};
    math::Color linear_factor;
    math::Color quad_factor;
};

struct alignas(16) PointLight : Light {
    alignas(16) math::Vector3 position;
    alignas(16) Attenuation attenuation;
};

class PointLightComponent : public LightComponent {
  public:
    struct Initializer : LightComponent::Initializer {
        Attenuation attenuation;
    };

    explicit PointLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] const Attenuation& Attenuation() const;
    [[nodiscard]] class Attenuation& Attenuation();

    [[nodiscard]] PointLight PointLight() const;

    [[nodiscard]] math::Matrix4x4 ProjectionMatrix() const override;

  private:
    class Attenuation attenuation_;
};

struct alignas(16) SpotLight : Light {
    alignas(16) math::Vector3 direction;
    alignas(16) math::Vector3 position;
    alignas(16) Attenuation attenuation;
    float inner_cone_angle;
    float outer_cone_angle;
};

class SpotLightComponent : public LightComponent {
  public:
    struct Initializer : LightComponent::Initializer {
        Attenuation attenuation;
        float inner_cone_angle{};
        float outer_cone_angle{};

        [[nodiscard]] math::Vector3 Direction() const;
        void Direction(const math::Vector3& direction);
    };

    explicit SpotLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] math::Vector3 Direction() const;
    void Direction(const math::Vector3& direction);

    [[nodiscard]] const Attenuation& Attenuation() const;
    [[nodiscard]] class Attenuation& Attenuation();

    [[nodiscard]] float InnerConeAngle() const;
    [[nodiscard]] float& InnerConeAngle();

    [[nodiscard]] float OuterConeAngle() const;
    [[nodiscard]] float& OuterConeAngle();

    [[nodiscard]] SpotLight SpotLight() const;

    [[nodiscard]] math::Matrix4x4 ProjectionMatrix() const override;

  private:
    class Attenuation attenuation_;
    float inner_cone_angle_;
    float outer_cone_angle_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_LIGHT_HPP_INCLUDED
