#include "sun.hpp"

#include <borov_engine/game.hpp>

// namespace detail {
//
// borov_engine::GeometricPrimitiveComponent& CreateSunMesh(borov_engine::Game& game,
//                                                          const borov_engine::SceneComponent* parent = nullptr) {
//     borov_engine::SphereGeometricPrimitiveArguments arguments{
//         .diameter = 2.0f,
//     };
//     borov_engine::Transform transform;
//     borov_engine::math::Color color{borov_engine::math::colors::linear::Yellow};
//     color *= 2.0f;
//     return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(arguments, transform, parent, color);
// }

// }  // namespace detail

Sun::Sun(borov_engine::Game& game, const borov_engine::Transform& transform, const SceneComponent* parent)
    : SceneComponent(game, transform, parent),
      mesh_{
          Game().AddComponent<borov_engine::BoxComponent>(
              1.5f, 1.5f, 1.5f, borov_engine::math::Color{borov_engine::math::colors::linear::Yellow},
              borov_engine::Transform{}, this),
      } {}

const borov_engine::BoxComponent& Sun::Mesh() const {
    return mesh_;
}

borov_engine::BoxComponent& Sun::Mesh() {
    return mesh_;
}

borov_engine::math::AxisAlignedBox Sun::BoxCollision() const {
    auto [position, rotation, scale] = WorldTransform();
    return borov_engine::math::AxisAlignedBox{
        position,
        borov_engine::math::Vector3::One * 0.75f * scale,
    };
}
