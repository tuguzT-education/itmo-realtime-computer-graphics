#include "cheese.hpp"

Cheese::Cheese(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/cheese/cheese.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 30.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Cheese::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Cheese::Mesh() {
    return mesh_;
}

void Cheese::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Cheese::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Cheese::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Cheese::CollisionPrimitive() const {
    const auto [position, rotation, scale] = WorldTransform();
    const borov_engine::math::Box box{
        position + borov_engine::math::Vector3{0.0f, 0.1f, 0.0f},
        scale * borov_engine::math::Vector3{0.1f, 0.1f, 0.1f},
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
