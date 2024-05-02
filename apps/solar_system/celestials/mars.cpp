#include "mars.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMarsMesh(borov_engine::Game& game,
                                                          const borov_engine::SceneComponent* parent) {
    borov_engine::IcosahedronGeometricPrimitiveArguments arguments{
        .size = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::OrangeRed};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Mars::Mars(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateMarsMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Mars::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Mars::Mesh() {
    return mesh_;
}

bool Mars::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Mars::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Mars::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.25f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
