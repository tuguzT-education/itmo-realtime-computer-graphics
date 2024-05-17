#pragma once

#ifndef KATAMARI_OBJECTS_TANTO_HPP_INCLUDED
#define KATAMARI_OBJECTS_TANTO_HPP_INCLUDED

#include <borov_engine/collision.hpp>
#include <borov_engine/mesh_component.hpp>

class Tanto final : public borov_engine::SceneComponent, public borov_engine::Collision {
  public:
    using MeshType = borov_engine::MeshComponent<>;

    explicit Tanto(borov_engine::Game& game, const Initializer& initializer = {});

    [[nodiscard]] const MeshType& Mesh() const;
    [[nodiscard]] MeshType& Mesh();

    void Draw(const borov_engine::Camera* camera) override;

    [[nodiscard]] bool Intersects(const Collision& other) const override;
    [[nodiscard]] bool Intersects(const borov_engine::math::Ray& ray, float& dist) const override;

  private:
    [[nodiscard]] borov_engine::BoxCollision CollisionPrimitive() const;

    std::reference_wrapper<MeshType> mesh_;
};

#endif  // KATAMARI_OBJECTS_TANTO_HPP_INCLUDED
