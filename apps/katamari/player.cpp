#include "player.hpp"

Player::Player(borov_engine::Game& game, const ControlKeys control_keys)
    : TriangleComponent(game,
                        [] {
                            MeshInitializer initializer{.mesh_path = "resources/meshes/katamari/katamari.fbx"};
                            initializer.Transform({
                                .position = borov_engine::math::Vector3::Up / 2.0f,
                                .scale = borov_engine::math::Vector3::One / 200.0f,
                            });
                            return initializer;
                        }()),
      control_keys_{control_keys} {}

auto Player::Controls() const -> ControlKeys {
    return control_keys_;
}

auto Player::Controls() -> ControlKeys& {
    return control_keys_;
}

void Player::Draw(const borov_engine::Camera* camera) {
    TriangleComponent::Draw(camera);

    Game().DebugDraw().DrawSphere(CollisionPrimitive().Primitive());
}

bool Player::Intersects(const Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Player::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Player::CollisionPrimitive() const {
    const borov_engine::math::Sphere sphere{WorldTransform().position, 0.5f};
    return borov_engine::SphereCollision{sphere};
}
