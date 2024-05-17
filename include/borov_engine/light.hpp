#pragma once

#ifndef BOROV_ENGINE_LIGHT_HPP_INCLUDED
#define BOROV_ENGINE_LIGHT_HPP_INCLUDED

#include "component.hpp"
#include "math.hpp"

namespace borov_engine {

struct alignas(16) AmbientLight {
    math::Color color;
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

}  // namespace borov_engine

#endif  // BOROV_ENGINE_LIGHT_HPP_INCLUDED
