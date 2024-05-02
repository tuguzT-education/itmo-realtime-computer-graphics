#include "borov_engine/box_component.hpp"

#include <array>

namespace borov_engine {

namespace detail {

std::array<BoxComponent::Vertex, 8> BoxVertices(const float length, const float height, const float width,
                                                const math::Color color) {
    const float right = length / 2;
    const float left = -length / 2;
    const float top = height / 2;
    const float bottom = -height / 2;
    const float forward = width / 2;
    const float backward = -width / 2;
    return {
        BoxComponent::Vertex{math::Vector3{left, top, backward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{right, top, backward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{left, bottom, backward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{right, bottom, backward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{left, top, forward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{right, top, forward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{left, bottom, forward}, color, math::Vector2{}},
        BoxComponent::Vertex{math::Vector3{right, bottom, forward}, color, math::Vector2{}},
    };
}

std::array<BoxComponent::Index, 36> BoxIndices() {
    // clang-format off
    return {
        0, 1, 2, 2, 1, 3,
        4, 0, 6, 6, 0, 2,
        7, 5, 6, 6, 5, 4,
        3, 1, 7, 7, 1, 5,
        4, 5, 0, 0, 5, 1,
        3, 7, 2, 2, 7, 6,
    };
    // clang-format on
}

// VERY dangerous if not used properly
template <class T>
T &Unmove(T &&t) {
    return static_cast<T &>(t);
}

}  // namespace detail

BoxComponent::BoxComponent(class Game &game, const float length, const float height, const float width,
                           const math::Color color, const class Transform &transform, const SceneComponent *parent)
    : TriangleComponent(game, detail::Unmove(detail::BoxVertices(length, height, width, color)),
                        detail::Unmove(detail::BoxIndices()), false, transform, parent),
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

bool BoxComponent::Intersects(const class CollisionPrimitive &other) const {
    return CollisionPrimitive().Intersects(other);
}

bool BoxComponent::Intersects(const math::Ray &ray, float &dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

BoxCollisionPrimitive BoxComponent::CollisionPrimitive() const {
    const auto [center, orientation, scale] = WorldTransform();
    const math::Vector3 extents = math::Vector3{Length() / 2, Height() / 2, Width() / 2} * scale;

    const math::Box box{center, extents, orientation};
    return BoxCollisionPrimitive{box};
}

}  // namespace borov_engine
