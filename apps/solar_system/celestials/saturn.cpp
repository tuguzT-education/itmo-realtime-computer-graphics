#include "saturn.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateSaturnMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::DodecahedronGeometricPrimitiveArguments arguments{
        .size = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGoldenrodYellow};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Saturn::Saturn(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateSaturnMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Saturn::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Saturn::Mesh() {
    return mesh_;
}

bool Saturn::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Saturn::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Saturn::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.25f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
