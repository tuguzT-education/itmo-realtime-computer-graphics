#include "boat.hpp"

Boat::Boat(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/boat/boat.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 200.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Boat::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Boat::Mesh() {
    return mesh_;
}

void Boat::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Boat::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Boat::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Boat::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.1f, 0.0f},
        borov_engine::math::Vector3{0.25f, 0.1f, 1.0f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
