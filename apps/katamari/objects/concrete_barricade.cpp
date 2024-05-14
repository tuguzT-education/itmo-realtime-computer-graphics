#include "concrete_barricade.hpp"

ConcreteBarricade::ConcreteBarricade(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/concrete_barricade/concrete_barricade.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 100.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& ConcreteBarricade::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& ConcreteBarricade::Mesh() {
    return mesh_;
}

void ConcreteBarricade::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawBox(CollisionPrimitive().Primitive());
}

bool ConcreteBarricade::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool ConcreteBarricade::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision ConcreteBarricade::CollisionPrimitive() const {
    borov_engine::math::Box box{
        borov_engine::math::Vector3{0.0f, 0.5f, -0.34f},
        borov_engine::math::Vector3{0.1f, 0.5f, 0.85f},
        borov_engine::math::Quaternion::Identity,
    };
    box.Transform(box, WorldTransform().ToMatrix());

    return borov_engine::BoxCollision{box};
}