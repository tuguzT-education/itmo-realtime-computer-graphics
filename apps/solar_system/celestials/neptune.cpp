#include "neptune.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateNeptuneMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::SphereGeometricPrimitiveArguments{
                .diameter = 0.75f,
            },
        .color = borov_engine::math::colors::linear::BlueViolet.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Neptune::Neptune(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateNeptuneMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() {
    return mesh_;
}

bool Neptune::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Neptune::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Neptune::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.375f};
    return borov_engine::SphereCollision{sphere};
}
