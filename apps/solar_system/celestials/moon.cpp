#include "moon.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMoonMesh(borov_engine::Game& game,
                                                          const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::TetrahedronGeometricPrimitiveArguments{
                .size = 0.1f,
            },
        .color = borov_engine::math::colors::linear::LightGray.v,
    };
    initializer.parent = parent;
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Moon::Moon(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateMoonMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Moon::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Moon::Mesh() {
    return mesh_;
}

void Moon::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    const auto collision_primitive = CollisionPrimitive();
    const auto& sphere = collision_primitive.Primitive();
    Game().DebugDraw().DrawSphere(sphere);
}

bool Moon::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Moon::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Moon::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.05f};
    return borov_engine::SphereCollision{sphere};
}
