#include "jupyter.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateJupyterMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 1.0f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::DarkOrange};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Jupyter::Jupyter(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateJupyterMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Jupyter::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Jupyter::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.5f * scale,
    };
}
