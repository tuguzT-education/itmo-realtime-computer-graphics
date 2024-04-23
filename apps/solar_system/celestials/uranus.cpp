#include "uranus.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateUranusMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightSkyBlue};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Uranus::Uranus(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateUranusMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Uranus::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Uranus::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Uranus::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.25f * scale,
    };
}
