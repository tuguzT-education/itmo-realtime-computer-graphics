#include "tanto.hpp"

Tanto::Tanto(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/tanto/tanto.fbx",
              };
              mesh_initializer.transform = borov_engine::Transform{
                  .position = borov_engine::math::Vector3::Up * 0.06f,
                  .scale = borov_engine::math::Vector3::One / 40.0f,
              };
              mesh_initializer.parent = this;
              return mesh_initializer;
          }()),
      } {}

auto Tanto::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Tanto::Mesh() -> MeshType& {
    return mesh_;
}

void Tanto::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Tanto::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Tanto::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Tanto::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.05f, 0.0f},
        borov_engine::math::Vector3{0.01f, 0.05f, 0.35f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
