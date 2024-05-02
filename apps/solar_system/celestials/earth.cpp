#include "earth.hpp"

#include <borov_engine/game.hpp>

Earth::Earth(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent),
      mesh_{
          Game().AddComponent<borov_engine::BoxComponent>(
              0.5f, 0.5f, 0.5f, borov_engine::math::Color{borov_engine::math::colors::linear::SpringGreen}, nullptr,
              borov_engine::Transform{}, this),
      } {}

const borov_engine::BoxComponent& Earth::Mesh() const {
    return mesh_;
}

borov_engine::BoxComponent& Earth::Mesh() {
    return mesh_;
}

bool Earth::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Earth::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Earth::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.25f * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
