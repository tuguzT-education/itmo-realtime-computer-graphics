#include "neptune.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateNeptuneMesh(borov_engine::Game& game,
                                                             const borov_engine::SceneComponent* parent = nullptr) {
    borov_engine::SphereGeometricPrimitiveArguments arguments{
        .diameter = 0.75f,
    };
    borov_engine::Transform transform;
    borov_engine::math::Color color{borov_engine::math::colors::linear::BlueViolet};
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
}

}  // namespace detail

Neptune::Neptune(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent), mesh_{detail::CreateNeptuneMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Neptune::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Neptune::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.375f * scale,
    };
}
