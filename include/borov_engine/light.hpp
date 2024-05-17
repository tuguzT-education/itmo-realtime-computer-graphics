#pragma once

#ifndef BOROV_ENGINE_LIGHT_HPP_INCLUDED
#define BOROV_ENGINE_LIGHT_HPP_INCLUDED

#include "component.hpp"
#include "math.hpp"

namespace borov_engine {

struct DirectionalLight {
    alignas(16) math::Vector3 direction = math::Normalize(math::Vector3::Down + math::Vector3::Forward);
    alignas(16) math::Color color{math::colors::linear::White};
};

class DirectionalLightComponent : public Component {
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
