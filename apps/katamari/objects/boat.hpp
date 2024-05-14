#pragma once

#ifndef KATAMARI_OBJECTS_BOAT_HPP_INCLUDED
#define KATAMARI_OBJECTS_BOAT_HPP_INCLUDED

#include <borov_engine/collision.hpp>
#include <borov_engine/triangle_component.hpp>

class Boat final : public borov_engine::SceneComponent, public borov_engine::Collision {
  public:
    explicit Boat(borov_engine::Game& game, const Initializer& initializer = {});

    [[nodiscard]] const borov_engine::TriangleComponent& Mesh() const;
    [[nodiscard]] borov_engine::TriangleComponent& Mesh();

    void Draw(const borov_engine::Camera* camera) override;

    [[nodiscard]] bool Intersects(const Collision& other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollision CollisionPrimitive() const;

    std::reference_wrapper<borov_engine::TriangleComponent> mesh_;
};

#endif  // KATAMARI_OBJECTS_BOAT_HPP_INCLUDED
