#include "strawberry.hpp"

Strawberry::Strawberry(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/strawberry/strawberry.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 2000.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

auto Strawberry::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Strawberry::Mesh() -> MeshType& {
    return mesh_;
}

void Strawberry::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawSphere(CollisionPrimitive().Primitive());
}

bool Strawberry::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Strawberry::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Strawberry::CollisionPrimitive() const {
    borov_engine::math::Sphere sphere{
        borov_engine::math::Vector3{0.0f, 0.175f, 0.0f},
        0.125f,
    };
    sphere.Transform(sphere, WorldTransform().ToMatrix());

    return borov_engine::SphereCollision{sphere};
}
