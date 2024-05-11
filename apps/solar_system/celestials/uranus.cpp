#include "uranus.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateUranusMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::SphereGeometricPrimitiveArguments{
                .diameter = 0.5f,
            },
        .color = borov_engine::math::colors::linear::LightSkyBlue.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Uranus::Uranus(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateUranusMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Uranus::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Uranus::Mesh() {
    return mesh_;
}

bool Uranus::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Uranus::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Uranus::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.25f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
