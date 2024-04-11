#include "square_component.hpp"

#include <array>

namespace detail {

std::array vertices{
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, 0.5f, 0.0f},
        borov_engine::math::Color{1.0f, 0.0f, 0.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, -0.5f, 0.0f},
        borov_engine::math::Color{0.0f, 0.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, -0.5f, 0.0f},
        borov_engine::math::Color{0.0f, 1.0f, 0.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, 0.5f, 0.0f},
        borov_engine::math::Color{1.0f, 1.0f, 1.0f},
    },
};

std::array<SquareComponent::Index, 6> indices{0, 1, 2, 1, 0, 3};

}  // namespace detail

SquareComponent::SquareComponent(borov_engine::Game &game)
    : TriangleComponent{game, detail::vertices, detail::indices} {}

void SquareComponent::Update(const float delta_time) {
    auto &position = Transform().position;
    position.x += 0.5f * delta_time;
    if (position.x > 1.5f) {
        position.x -= 3.0f;
    }
}
