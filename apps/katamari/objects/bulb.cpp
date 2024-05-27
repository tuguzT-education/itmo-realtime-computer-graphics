#include "bulb.hpp"

Bulb::Bulb(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<MeshType>([this] {
              MeshType::Initializer mesh_initializer{
                  .mesh_path = "resources/meshes/bulb/bulb.fbx",
              };
              mesh_initializer.transform = {.scale = borov_engine::math::Vector3::One / 50.0f};
              mesh_initializer.parent = this;
              return mesh_initializer;
          }()),
      } {}

auto Bulb::Mesh() const -> const MeshType& {
    return mesh_;
}

auto Bulb::Mesh() -> MeshType& {
    return mesh_;
}

void Bulb::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool Bulb::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Bulb::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Bulb::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.125f, 0.0f},
        borov_engine::math::Vector3{0.025f, 0.125f, 0.025f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}
