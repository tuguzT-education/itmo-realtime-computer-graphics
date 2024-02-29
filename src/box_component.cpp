#include "borov_engine/box_component.hpp"

#include <array>

namespace borov_engine {

namespace detail {

std::array<BoxComponent::Vertex, 4> BoxVertices(float width, float height, math::Color color) {
    float left = -width / 2;
    float top = height / 2;
    float right = width / 2;
    float bottom = -height / 2;
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

template<class T>
T &Unmove(T &&t) {
    return static_cast<T &>(t);
}

}

BoxComponent::BoxComponent(Game &game, float width, float height, math::Color color, math::Vector3 position)
    : TriangleComponent(game,
                        detail::Unmove(detail::BoxVertices(width, height, color)),
                        detail::Unmove(detail::BoxIndices()),
                        position),
      width_{width},
      height_{height} {}

float BoxComponent::Width() const {
    return width_;
}

float BoxComponent::Height() const {
    return height_;
}

BoxComponent::Box BoxComponent::Collision() const {
    math::Vector3 center = Position();
    math::Vector3 extents = math::Vector3{Width() / 2, Height() / 2, 0.0f};
    return Box{center, extents};
}

}
