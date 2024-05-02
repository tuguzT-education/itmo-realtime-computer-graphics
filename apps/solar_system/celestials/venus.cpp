#include "venus.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateVenusMesh(borov_engine::Game& game,
                                                           const borov_engine::SceneComponent* parent) {
    borov_engine::CylinderGeometricPrimitiveArguments arguments{
        .height = 0.25f,
        .diameter = 0.25f,
    };
    borov_engine::Transform transform{
        .rotation = borov_engine::math::Quaternion::CreateFromAxisAngle(borov_engine::math::Vector3::Right,
                                                                        std::numbers::pi_v<float> / 2.0f),
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightYellow};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, color, "", false, transform, parent);
}

}  // namespace detail

Venus::Venus(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateVenusMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Venus::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Venus::Mesh() {
    return mesh_;
}

bool Venus::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Venus::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollisionPrimitive Venus::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.125f * scale,
        rotation,
    };
    return borov_engine::BoxCollisionPrimitive{box};
}
