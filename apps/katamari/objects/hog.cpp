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

void Hog::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
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
        position + borov_engine::math::Vector3{0.0f, 0.5f, 0.25f},
        borov_engine::math::Vector3{0.35f, 0.35f, 0.85f} * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
