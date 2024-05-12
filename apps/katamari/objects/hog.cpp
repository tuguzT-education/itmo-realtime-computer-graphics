#include "hog.hpp"

Hog::Hog(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/hog.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 200.0f});
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Hog::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Hog::Mesh() {
    return mesh_;
}

bool Hog::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Hog::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Hog::CollisionPrimitive() const {
    const auto [position, rotation, scale] = WorldTransform();
    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3{1.0f, 0.75f, 1.5f} * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
