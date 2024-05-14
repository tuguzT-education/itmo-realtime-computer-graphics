#pragma once

#ifndef KATAMARI_OBJECTS_APRICOT_HPP_INCLUDED
#define KATAMARI_OBJECTS_APRICOT_HPP_INCLUDED

#include <borov_engine/collision_primitive.hpp>
#include <borov_engine/triangle_component.hpp>

class Apricot final : public borov_engine::SceneComponent, public borov_engine::CollisionPrimitive {
  public:
    explicit Apricot(borov_engine::Game& game, const Initializer& initializer = {});

    [[nodiscard]] const borov_engine::TriangleComponent& Mesh() const;
    [[nodiscard]] borov_engine::TriangleComponent& Mesh();

    void Draw(const borov_engine::Camera* camera) override;

    [[nodiscard]] bool Intersects(const CollisionPrimitive& other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] borov_engine::SphereCollisionPrimitive CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::TriangleComponent> mesh_;
};

#endif  // KATAMARI_OBJECTS_APRICOT_HPP_INCLUDED
