#include "borov_engine/box_component.hpp"

#include <array>

namespace borov_engine {

namespace detail {

std::array<BoxComponent::Vertex, 4> BoxVertices(float length, float height, float width, math::Color color) {
    float right = length / 2;
    float left = -length / 2;
    float top = height / 2;
    float bottom = -height / 2;
    float forward = width / 2;
    float backward = -width / 2;
    return {
        BoxComponent::Vertex{{right, top, 0.0f}, color},
        BoxComponent::Vertex{{left, bottom, 0.0f}, color},
        BoxComponent::Vertex{{right, bottom, 0.0f}, color},
        BoxComponent::Vertex{{left, top, 0.0f}, color},
    };
}

std::array<BoxComponent::Index, 6> BoxIndices() {
    return {0, 1, 2, 1, 0, 3};
}

// VERY dangerous if not used properly
template<class T>
inline T &Unmove(T &&t) {
    return static_cast<T &>(t);
}

}

BoxComponent::BoxComponent(Game &game,
                           float length, float height, float width,
                           math::Color color, math::Vector3 position)
    : TriangleComponent(game,
                        detail::Unmove(detail::BoxVertices(length, height, width, color)),
                        detail::Unmove(detail::BoxIndices()),
                        position),
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
    math::Vector3 center = Position();
    math::Vector3 extents = math::Vector3{Length() / 2, Height() / 2, Width() / 2};
    return Box{center, extents};
}

}
