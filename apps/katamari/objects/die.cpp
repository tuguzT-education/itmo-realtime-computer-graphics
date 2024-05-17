#include "die.hpp"

Die::Die(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/die/red/die.fbx",
              };
              mesh_initializer.Transform({
                  .position = borov_engine::math::Vector3::Up * 0.125f,
                  .scale = borov_engine::math::Vector3::One / 400.0f,
              });
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

auto Die::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Die::Mesh() -> MeshType& {
    return mesh_;
}

void Die::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Die::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Die::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Die::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.125f, 0.0f},
        borov_engine::math::Vector3{0.125f, 0.125f, 0.125f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
