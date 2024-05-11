#pragma once

#ifndef BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED

#include "collision_primitive.hpp"
#include "triangle_component.hpp"

namespace borov_engine {

class BoxComponent : public TriangleComponent, public CollisionPrimitive {
  public:
    struct Initializer : TriangleComponent::Initializer {
        float length = 1.0f;
        float height = 1.0f;
        float width = 1.0f;
        math::Color color{math::colors::linear::White};
        std::filesystem::path texture_path;

        Initializer& Length(float length);
        Initializer& Height(float height);
        Initializer& Width(float width);
        Initializer& Color(math::Color color);
        Initializer& TexturePath(const std::filesystem::path& texture_path);
    };

    explicit BoxComponent(class Game& game, const Initializer& initializer = {});

    [[nodiscard]] float Length() const;
    [[nodiscard]] float Height() const;
    [[nodiscard]] float Width() const;

    [[nodiscard]] bool Intersects(const CollisionPrimitive& other) const override;
    [[nodiscard]] bool Intersects(const math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] BoxCollisionPrimitive CollisionPrimitive() const;

    float length_;
    float height_;
    float width_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_BOX_COMPONENT_HPP_INCLUDED
