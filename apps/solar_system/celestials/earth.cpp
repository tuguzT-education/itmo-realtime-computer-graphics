#include "earth.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateEarthMesh(borov_engine::Game& game,
                                                           const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::GeoSphereGeometricPrimitiveArguments arguments{
        .diameter = 0.5f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::SpringGreen};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Earth::Earth(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateEarthMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Earth::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Earth::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Earth::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.25f * scale,
    };
}
