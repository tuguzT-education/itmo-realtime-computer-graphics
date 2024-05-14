#include "axe.hpp"

Axe::Axe(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/axe/axe.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 50.0f});
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
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.01f, 0.03f, -0.08f},
        borov_engine::math::Vector3{0.55f, 0.02f, 0.04f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollisionPrimitive{box};
}
