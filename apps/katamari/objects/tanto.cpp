#include "tanto.hpp"

Tanto::Tanto(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/tanto/tanto.fbx",
              };
              mesh_initializer.Transform({
                  .position = borov_engine::math::Vector3::Up * 0.06f,
                  .scale = borov_engine::math::Vector3::One / 40.0f,
              });
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Tanto::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Tanto::Mesh() {
    return mesh_;
}

void Tanto::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Tanto::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Tanto::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Tanto::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.05f, 0.0f},
        borov_engine::math::Vector3{0.01f, 0.05f, 0.35f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollisionPrimitive{box};
}
