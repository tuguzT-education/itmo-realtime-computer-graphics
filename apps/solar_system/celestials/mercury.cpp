#include "mercury.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMercuryMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::TorusGeometricPrimitiveArguments arguments{
        .diameter = 0.2f,
        .thickness = 0.1f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::SlateGray};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Mercury::Mercury(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateMercuryMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Mercury::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Mercury::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Mercury::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.1f * scale,
    };
}
