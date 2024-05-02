#include "moon.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMoonMesh(borov_engine::Game& game,
                                                          const borov_engine::SceneComponent* parent) {
    borov_engine::TetrahedronGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGray};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Moon::Moon(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateMoonMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Moon::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Moon::Mesh() {
    return mesh_;
}

bool Moon::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Moon::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Moon::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.05f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
