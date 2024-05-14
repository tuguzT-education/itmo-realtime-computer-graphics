#include "chair.hpp"

Chair::Chair(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/chair/chair.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 40.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Chair::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Chair::Mesh() {
    return mesh_;
}

void Chair::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Chair::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Chair::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Chair::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.35f, 0.0f},
        borov_engine::math::Vector3{0.4f, 0.35f, 0.4f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
