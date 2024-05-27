#include "deimos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateDeimosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::BoxGeometricPrimitiveArguments{
                .size = borov_engine::math::Vector3::One * 0.1f,
            },
        .color = borov_engine::math::colors::linear::DarkRed.v,
    };
    initializer.parent = parent;
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Deimos::Deimos(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateDeimosMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() {
    return mesh_;
}

bool Deimos::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Deimos::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Deimos::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.05f * scale,
        rotation,
    };
    return borov_engine::BoxCollision{box};
}
