#include "square_component.hpp"

#include <array>

namespace detail {

const std::array vertices{
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, 0.5f, 0.0f},
        borov_engine::math::Vector3{0.0f, 0.0f, 1.0f},
        borov_engine::math::colors::linear::Red,
        borov_engine::math::Vector2{1.0f, 0.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, -0.5f, 0.0f},
        borov_engine::math::Vector3{0.0f, 0.0f, 1.0f},
        borov_engine::math::colors::linear::Blue,
        borov_engine::math::Vector2{0.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, -0.5f, 0.0f},
        borov_engine::math::Vector3{0.0f, 0.0f, 1.0f},
        borov_engine::math::colors::linear::Lime,
        borov_engine::math::Vector2{1.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, 0.5f, 0.0f},
        borov_engine::math::Vector3{0.0f, 0.0f, 1.0f},
        borov_engine::math::colors::linear::White,
        borov_engine::math::Vector2{0.0f, 0.0f},
    },
};

constexpr std::array<SquareComponent::Index, 6> indices{0, 1, 2, 1, 0, 3};

}  // namespace detail

SquareComponent::SquareComponent(borov_engine::Game &game)
    : TriangleComponent(game, Initializer{.vertices = detail::vertices, .indices = detail::indices}) {}

void SquareComponent::Update(const float delta_time) {
    auto &position = Transform().position;
    position.x += 0.5f * delta_time;
    if (position.x > 1.5f) {
        position.x -= 3.0f;
    }
}
