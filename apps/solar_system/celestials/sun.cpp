#include "sun.hpp"

#include <borov_engine/game.hpp>

Sun::Sun(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent),
      mesh_{
          Game().AddComponent<borov_engine::BoxComponent>(
              1.5f, 1.5f, 1.5f, borov_engine::math::Color{borov_engine::math::colors::linear::Yellow}, "",
              borov_engine::Transform{}, this),
      } {}

const borov_engine::BoxComponent& Sun::Mesh() const {
    return mesh_;
}

borov_engine::BoxComponent& Sun::Mesh() {
    return mesh_;
}

bool Sun::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Sun::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Sun::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.75f * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
