#include "axe.hpp"

Axe::Axe(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/axe/axe.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 100.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Axe::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Axe::Mesh() {
    return mesh_;
}

void Axe::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Axe::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Axe::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Axe::CollisionPrimitive() const {
    const auto [position, rotation, scale] = WorldTransform();
    const borov_engine::math::Box box{
        position + borov_engine::math::Vector3{0.005f, 0.015f, -0.04f},
        scale * borov_engine::math::Vector3{0.275f, 0.01f, 0.02f},
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
