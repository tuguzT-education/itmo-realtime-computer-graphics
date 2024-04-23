#include "phobos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreatePhobosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent = nullptr) {
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

borov_engine::math::AxisAlignedBox Phobos::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.1f * scale,
    };
}
