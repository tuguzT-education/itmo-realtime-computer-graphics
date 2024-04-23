#include "deimos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateDeimosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::BoxGeometricPrimitiveArguments arguments{
        .size = borov_engine::math::Vector3::One * 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkRed};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Deimos::Deimos(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateDeimosMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Deimos::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Deimos::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.05f * scale,
    };
}
