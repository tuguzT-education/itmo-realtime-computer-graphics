#include "phobos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreatePhobosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::TeapotGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::PaleVioletRed};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Phobos::Phobos(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreatePhobosMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Phobos::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Phobos::Mesh() {
    return mesh_;
}

bool Phobos::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Phobos::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Phobos::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.1f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
