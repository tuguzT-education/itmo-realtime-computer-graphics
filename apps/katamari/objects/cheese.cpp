#include "cheese.hpp"

Cheese::Cheese(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/cheese/cheese.fbx",
              };
              mesh_initializer.transform = {.scale = borov_engine::math::Vector3::One / 30.0f};
              mesh_initializer.parent = this;
              return mesh_initializer;
          }()),
      } {}

auto Cheese::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Cheese::Mesh() -> MeshType& {
    return mesh_;
}

void Cheese::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Cheese::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Cheese::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Cheese::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.1f, 0.0f},
        borov_engine::math::Vector3{0.1f, 0.1f, 0.1f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
