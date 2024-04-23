#include "venus.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateVenusMesh(borov_engine::Game& game,
                                                           const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::CylinderGeometricPrimitiveArguments arguments{
        .height = 0.25f,
        .diameter = 0.25f,
    };
    borov_engine::Transform transform{
        .rotation = borov_engine::math::Quaternion::CreateFromAxisAngle(borov_engine::math::Vector3::Right,
                                                                        std::numbers::pi_v<float> / 2.0f),
    };
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightYellow};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
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

borov_engine::math::AxisAlignedBox Venus::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.125f * scale,
    };
}
