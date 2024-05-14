#include "jupyter.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateJupyterMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::SphereGeometricPrimitiveArguments{
                .diameter = 1.0f,
            },
        .color = borov_engine::math::colors::linear::DarkOrange.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Jupyter::Jupyter(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateJupyterMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() {
    return mesh_;
}

bool Jupyter::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Jupyter::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Jupyter::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.5f};
    return borov_engine::SphereCollision{sphere};
}
