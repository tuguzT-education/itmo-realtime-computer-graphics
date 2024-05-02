#include "jupyter.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateJupyterMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent) {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 1.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkOrange};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Jupyter::Jupyter(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateJupyterMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() {
    return mesh_;
}

bool Jupyter::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Jupyter::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Jupyter::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.5f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
