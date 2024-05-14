#include "venus.hpp"

#include <borov_engine/game.hpp>
#include <numbers>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateVenusMesh(borov_engine::Game& game,
                                                           const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::CylinderGeometricPrimitiveArguments{
                .height = 0.25f,
                .diameter = 0.25f,
            },
        .color = borov_engine::math::colors::linear::LightYellow.v,
    };
    initializer
        .Transform({
            .rotation = borov_engine::math::Quaternion::CreateFromAxisAngle(borov_engine::math::Vector3::Right,
                                                                            std::numbers::pi_v<float> / 2.0f),
        })
        .Parent(parent);
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Venus::Venus(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateVenusMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Venus::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Venus::Mesh() {
    return mesh_;
}

bool Venus::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Venus::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Venus::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.125f * scale,
        rotation,
    };
    return borov_engine::BoxCollision{box};
}
