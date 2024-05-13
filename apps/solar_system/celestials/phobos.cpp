#include "phobos.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreatePhobosMesh(borov_engine::Game& game,
                                                            const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::TeapotGeometricPrimitiveArguments{
                .size = 0.1f,
            },
        .color = borov_engine::math::colors::linear::PaleVioletRed.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Phobos::Phobos(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreatePhobosMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Phobos::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Phobos::Mesh() {
    return mesh_;
}

void Phobos::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    const auto collision_primitive = CollisionPrimitive();
    const auto& sphere = collision_primitive.Primitive();
    Game().DebugDraw().DrawSphere(sphere);
}

bool Phobos::Intersects(const borov_engine::CollisionPrimitive& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Phobos::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollisionPrimitive Phobos::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.1f};
    return borov_engine::SphereCollisionPrimitive{sphere};
}
