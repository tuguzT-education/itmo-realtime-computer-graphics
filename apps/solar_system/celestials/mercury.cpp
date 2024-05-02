#include "mercury.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMercuryMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent) {
    borov_engine::TorusGeometricPrimitiveArguments arguments{
        .diameter = 0.2f,
        .thickness = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::SlateGray};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Mercury::Mercury(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateMercuryMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Mercury::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Mercury::Mesh() {
    return mesh_;
}

bool Mercury::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Mercury::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Mercury::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.125f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
