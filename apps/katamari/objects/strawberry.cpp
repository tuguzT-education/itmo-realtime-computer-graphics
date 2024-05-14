#include "strawberry.hpp"

Strawberry::Strawberry(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/strawberry/strawberry.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 2000.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Strawberry::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Strawberry::Mesh() {
    return mesh_;
}

void Strawberry::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawSphere(CollisionPrimitive().Primitive());
}

bool Strawberry::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Strawberry::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Strawberry::CollisionPrimitive() const {
    const auto [position, rotation, scale] = WorldTransform();
    const borov_engine::math::Sphere sphere{
        position + borov_engine::math::Vector3{0.0f, 0.175f, 0.0f},
        0.125f,
    };
    return borov_engine::SphereCollisionPrimitive{sphere};
}
