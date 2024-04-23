#include "saturn.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateSaturnMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::DodecahedronGeometricPrimitiveArguments arguments{
        .size = 0.25f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::LightGoldenrodYellow};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Saturn::Saturn(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateSaturnMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Saturn::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Saturn::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Saturn::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.25f * scale,
    };
}
