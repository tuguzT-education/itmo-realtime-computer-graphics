#pragma once

#ifndef BOROV_ENGINE_LIGHT_HPP_INCLUDED
#define BOROV_ENGINE_LIGHT_HPP_INCLUDED

#include "scene_component.hpp"

namespace borov_engine {

struct alignas(16) AmbientLight {
    math::Color color{math::colors::linear::White};
};

class AmbientLightComponent final : public Component {
  public:
    using PrimitiveType = AmbientLight;

    struct Initializer : Component::Initializer {
        PrimitiveType light;
    };

    explicit AmbientLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] const PrimitiveType& Primitive() const noexcept;
    [[nodiscard]] PrimitiveType& Primitive() noexcept;

  private:
    PrimitiveType light_;
};

struct alignas(16) DirectionalLight {
    alignas(16) math::Vector3 direction;
    alignas(16) math::Color color;
};

class DirectionalLightComponent final : public Component {
  public:
    using PrimitiveType = DirectionalLight;

    struct Initializer : Component::Initializer {
        PrimitiveType light;
    };

    explicit DirectionalLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] const PrimitiveType& Primitive() const noexcept;
    [[nodiscard]] PrimitiveType& Primitive() noexcept;

  private:
    PrimitiveType light_;
};

struct alignas(16) Attenuation {
    math::Color const_factor{math::colors::linear::White};
    math::Color linear_factor;
    math::Color quad_factor;
};

struct alignas(16) PointLight {
    alignas(16) math::Vector3 position;
    alignas(16) math::Color color;
    alignas(16) Attenuation attenuation;
};

class PointLightComponent final : public SceneComponent {
  public:
    using PrimitiveType = PointLight;

    struct Initializer : SceneComponent::Initializer {
        math::Color color;
        Attenuation attenuation;
    };

    explicit PointLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] PrimitiveType Primitive() const;

    [[nodiscard]] math::Color Color() const;
    [[nodiscard]] math::Color& Color();

    [[nodiscard]] const Attenuation& Attenuation() const;
    [[nodiscard]] class Attenuation& Attenuation();

  private:
    math::Color color_;
    class Attenuation attenuation_;
};

struct alignas(16) SpotLight {
    alignas(16) math::Vector3 position;
    alignas(16) math::Vector3 direction;
    alignas(16) math::Color color;
    alignas(16) Attenuation attenuation;
    float inner_cone_angle;
    float outer_cone_angle;
};

class SpotLightComponent final : public SceneComponent {
  public:
    using PrimitiveType = SpotLight;

    struct Initializer : SceneComponent::Initializer {
        math::Color color;
        Attenuation attenuation;
        float inner_cone_angle{};
        float outer_cone_angle{};
    };

    explicit SpotLightComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] PrimitiveType Primitive() const;

    [[nodiscard]] math::Vector3 Direction() const;

    [[nodiscard]] math::Color Color() const;
    [[nodiscard]] math::Color& Color();

    [[nodiscard]] const Attenuation& Attenuation() const;
    [[nodiscard]] class Attenuation& Attenuation();

    [[nodiscard]] float InnerConeAngle() const;
    [[nodiscard]] float& InnerConeAngle();

    [[nodiscard]] float OuterConeAngle() const;
    [[nodiscard]] float& OuterConeAngle();

  private:
    math::Color color_;
    class Attenuation attenuation_;
    float inner_cone_angle_;
    float outer_cone_angle_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_LIGHT_HPP_INCLUDED
