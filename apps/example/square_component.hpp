#pragma once

#ifndef APPS_SQUARE_COMPONENT_HPP_INCLUDED
#define APPS_SQUARE_COMPONENT_HPP_INCLUDED

#include <array>

#include "borov_engine/triangle_component.hpp"

class SquareComponent : public borov_engine::TriangleComponent {
  public:
    explicit SquareComponent(borov_engine::Game &game);

    void Update(float delta_time) override;

  private:
    static std::array<Vertex, 4> vertices;
    static std::array<Index, 6> indices;
};

SquareComponent::SquareComponent(borov_engine::Game &game) : TriangleComponent{game, vertices, indices} {}

void SquareComponent::Update(float delta_time) {
    auto &position = Position();
    position.x += 0.5f * delta_time;
    if (position.x > 1.5f) {
        position.x -= 3.0f;
    }
}

std::array<SquareComponent::Vertex, 4> SquareComponent::vertices{
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, 0.5f, 0.0f},
        borov_engine::math::Color{1.0f, 0.0f, 0.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, -0.5f, 0.0f},
        borov_engine::math::Color{0.0f, 0.0f, 1.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{0.5f, -0.5f, 0.0f},
        borov_engine::math::Color{0.0f, 1.0f, 0.0f, 1.0f},
    },
    borov_engine::TriangleComponent::Vertex{
        borov_engine::math::Vector3{-0.5f, 0.5f, 0.0f},
        borov_engine::math::Color{1.0f, 1.0f, 1.0f, 1.0f},
    },
};

std::array<SquareComponent::Index, 6> SquareComponent::indices{0, 1, 2, 1, 0, 3};

#endif //APPS_SQUARE_COMPONENT_HPP_INCLUDED
