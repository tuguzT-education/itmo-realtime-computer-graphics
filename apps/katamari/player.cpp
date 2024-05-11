#include "player.hpp"

Player::Player(borov_engine::Game& game, const ControlKeys control_keys)
    : GeometricPrimitiveComponent(game, borov_engine::SphereGeometricPrimitiveArguments{.tessellation = 32},
                                  borov_engine::math::colors::linear::White.v,
                                  "resources/textures/taurus-terazzo-white.jpg", false,
                                  borov_engine::Transform{.position = borov_engine::math::Vector3::Up / 2.0f}),
      control_keys_{control_keys} {}

auto Player::Controls() const -> ControlKeys {
    return control_keys_;
}

auto Player::Controls() -> ControlKeys& {
    return control_keys_;
}

bool Player::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Player::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Player::CollisionPrimitive() const {
    const borov_engine::math::Sphere sphere{WorldTransform().position, 0.5f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
