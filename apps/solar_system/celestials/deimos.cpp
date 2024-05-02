#include "deimos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateDeimosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::BoxGeometricPrimitiveArguments arguments{
        .size = borov_engine::math::Vector3::One * 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkRed};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Deimos::Deimos(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateDeimosMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() {
    return mesh_;
}

bool Deimos::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Deimos::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Deimos::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.05f * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
