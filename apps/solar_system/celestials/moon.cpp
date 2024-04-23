#include "moon.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMoonMesh(borov_engine::Game& game,
                                                          const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::TetrahedronGeometricPrimitiveArguments arguments{
        .size = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGray};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
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

borov_engine::math::AxisAlignedBox Moon::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.05f * scale,
    };
}
