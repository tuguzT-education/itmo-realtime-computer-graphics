#include "neptune.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateNeptuneMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent) {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 0.75f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::BlueViolet};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Neptune::Neptune(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateNeptuneMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() {
    return mesh_;
}

bool Neptune::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Neptune::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Neptune::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.375f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
