#include "earth.hpp"

#include <borov_engine/game.hpp>

namespace detail {

borov_engine::BoxComponent& CreateEarthMesh(borov_engine::Game& game, const borov_engine::SceneComponent* parent) {
    borov_engine::BoxComponent::Initializer initializer{
        .length = 0.5f,
        .height = 0.5f,
        .width = 0.5f,
        .color = borov_engine::math::colors::linear::SpringGreen.v,
    };
    initializer.parent = parent;
    return game.AddComponent<borov_engine::BoxComponent>(initializer);
}

}  // namespace detail

Earth::Earth(borov_engine::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreateEarthMesh(Game(), this)} {}

const borov_engine::BoxComponent& Earth::Mesh() const {
    return mesh_;
}

borov_engine::BoxComponent& Earth::Mesh() {
    return mesh_;
}

void Earth::Draw(const borov_engine::Camera* camera) {
    SceneComponent::Draw(camera);

    const auto collision_primitive = CollisionPrimitive();
    const auto& box = collision_primitive.Primitive();
    Game().DebugDraw().DrawBox(box);
}

bool Earth::Intersects(const borov_engine::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Earth::Intersects(const borov_engine::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

borov_engine::BoxCollision Earth::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();
    rotation.Normalize();

    const borov_engine::math::Box box{
        position,
        borov_engine::math::Vector3::One * 0.25f * scale,
        rotation,
    };
    return borov_engine::BoxCollision{box};
}
