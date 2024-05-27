#include "mars.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::GeometricPrimitiveComponent& CreateMarsMesh(borov_engine::Game& game,
                                                          const borov_engine::SceneComponent* parent) {
    borov_engine::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            borov_engine::IcosahedronGeometricPrimitiveArguments{
                .size = 0.25f,
            },
        .color = borov_engine::math::colors::linear::OrangeRed.v,
    };
    initializer.parent = parent;
    return game.AddComponent<borov_engine::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Mars::Mars(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateMarsMesh(Game(), this)} {}

const borov_engine::GeometricPrimitiveComponent& Mars::Mesh() const {
    return mesh_;
}

borov_engine::GeometricPrimitiveComponent& Mars::Mesh() {
    return mesh_;
}

bool Mars::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Mars::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::SphereCollision Mars::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const borov_engine::math::Sphere sphere{position, 0.25f};
    return borov_engine::SphereCollision{sphere};
}
