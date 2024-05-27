#include "axe.hpp"

Axe::Axe(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/axe/axe.fbx",
              };
              mesh_initializer.transform = {.scale = borov_engine::math::Vector3::One / 50.0f};
              mesh_initializer.parent = this;
              return mesh_initializer;
          }()),
      } {}

auto Axe::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Axe::Mesh() -> MeshType& {
    return mesh_;
}

void Axe::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Axe::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Axe::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Axe::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.01f, 0.03f, -0.08f},
        borov_engine::math::Vector3{0.55f, 0.02f, 0.04f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
