#include "apricot.hpp"

Apricot::Apricot(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      mesh_{
          Game().AddComponent<borov_engine::TriangleComponent>([this] {
              borov_engine::TriangleComponent::MeshInitializer mesh_initializer{
                  .mesh_path = "resources/meshes/apricot/apricot.fbx",
              };
              mesh_initializer.Transform({.scale = borov_engine::math::Vector3::One / 20.0f});
              mesh_initializer.Parent(this);
              return mesh_initializer;
          }()),
      } {}

const borov_engine::TriangleComponent& Apricot::Mesh() const {
    return mesh_;
}

borov_engine::TriangleComponent& Apricot::Mesh() {
    return mesh_;
}

void Apricot::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    Game().DebugDraw().DrawSphere(CollisionPrimitive().Primitive());
}

bool Apricot::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Apricot::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Apricot::CollisionPrimitive() const {
    borov_engine::math::Sphere sphere{
        borov_engine::math::Vector3{0.0f, 0.1375f, 0.0f},
        0.1f,
    };
    sphere.Transform(sphere, WorldTransform().ToMatrix());

    return borov_engine::SphereCollision{sphere};
}
