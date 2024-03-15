#include "borov_engine/box_component.hpp"

#include <array>

namespace borov_engine {

namespace detail {

std::array<BoxComponent::Vertex, 8> BoxVertices(float length, float height, float width, math::Color color) {
    float right = length / 2;
    float left = -length / 2;
    float top = height / 2;
    float bottom = -height / 2;
    float forward = width / 2;
    float backward = -width / 2;
    return {
        BoxComponent::Vertex{{left, top, backward}, color},
        BoxComponent::Vertex{{right, top, backward}, color},
        BoxComponent::Vertex{{left, bottom, backward}, color},
        BoxComponent::Vertex{{right, bottom, backward}, color},
        BoxComponent::Vertex{{left, top, forward}, color},
        BoxComponent::Vertex{{right, top, forward}, color},
        BoxComponent::Vertex{{left, bottom, forward}, color},
        BoxComponent::Vertex{{right, bottom, forward}, color},
    };
}

std::array<BoxComponent::Index, 36> BoxIndices() {
    return {
        0, 1, 2, 2, 1, 3,
        4, 0, 6, 6, 0, 2,
        7, 5, 6, 6, 5, 4,
        3, 1, 7, 7, 1, 5,
        4, 5, 0, 0, 5, 1,
        3, 7, 2, 2, 7, 6,
    };
}

// VERY dangerous if not used properly
template<class T>
inline T &Unmove(T &&t) {
    return static_cast<T &>(t);
}

}

BoxComponent::BoxComponent(borov_engine::Game &game,
                           float length, float height, float width, math::Color color,
                           borov_engine::Transform transform)
    : TriangleComponent(game,
                        detail::Unmove(detail::BoxVertices(length, height, width, color)),
                        detail::Unmove(detail::BoxIndices()),
                        transform),
      length_{length},
      height_{height},
      width_{width} {}

float BoxComponent::Length() const {
    return length_;
}

float BoxComponent::Height() const {
    return height_;
}

float BoxComponent::Width() const {
    return width_;
}

BoxComponent::Box BoxComponent::Collision() const {
    math::Vector3 center = Transform().position;
    math::Vector3 extents = math::Vector3{Length() / 2, Height() / 2, Width() / 2} * Transform().scale;
    math::Quaternion orientation = Transform().rotation;
    return Box{center, extents, orientation};
}

}
