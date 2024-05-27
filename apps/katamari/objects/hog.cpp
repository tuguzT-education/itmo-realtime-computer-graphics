#include "hog.hpp"

Hog::Hog(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/hog.fbx",
              };
              mesh_initializer.transform = {.scale = borov_engine::math::Vector3::One / 200.0f};
              mesh_initializer.parent = this;
              return mesh_initializer;
          }()),
      } {}

auto Hog::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Hog::Mesh() -> MeshType& {
    return mesh_;
}

void Hog::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Hog::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Hog::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Hog::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.5f, 0.15f},
        borov_engine::math::Vector3{0.3f, 0.35f, 0.8f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
