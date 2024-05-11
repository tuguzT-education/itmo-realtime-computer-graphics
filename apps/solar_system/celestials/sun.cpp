#include "sun.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::BoxComponent& CreateSunMesh(borov_engine::Game& game, const borov_engine::SceneComponent* parent) {
    borov_engine::BoxComponent::Initializer initializer{
        .length = 1.5f,
        .height = 1.5f,
        .width = 1.5f,
        .color = borov_engine::math::colors::linear::Yellow.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<borov_engine::BoxComponent>(initializer);
}

}  // namespace detail

Sun::Sun(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateSunMesh(Game(), this)} {}

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
