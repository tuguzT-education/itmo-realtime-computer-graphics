#include "borov_engine/box_component.hpp"

#include <array>

namespace borov_engine {

namespace detail {

std::array<BoxComponent::Vertex, 24> BoxVertices(const float length, const float height, const float width,
                                                 const math::Color color) {
    const float right = length / 2;
    const float left = -length / 2;
    const float top = height / 2;
    const float bottom = -height / 2;
    const float forward = -width / 2;
    const float backward = width / 2;

    return {
        // Front face
        BoxComponent::Vertex{math::Vector3{left, bottom, forward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{left, top, forward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, forward}, color, math::Vector2{1.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, bottom, forward}, color, math::Vector2{1.0f, 1.0f}},
        // Back face
        BoxComponent::Vertex{math::Vector3{left, bottom, backward}, color, math::Vector2{1.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{right, bottom, backward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, backward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{left, top, backward}, color, math::Vector2{1.0f, 0.0f}},
        // Top Face
        BoxComponent::Vertex{math::Vector3{left, top, forward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{left, top, backward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, backward}, color, math::Vector2{1.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, forward}, color, math::Vector2{1.0f, 1.0f}},
        // Bottom Face
        BoxComponent::Vertex{math::Vector3{left, bottom, forward}, color, math::Vector2{1.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{right, bottom, forward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{right, bottom, backward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{left, bottom, backward}, color, math::Vector2{1.0f, 0.0f}},
        // Left Face
        BoxComponent::Vertex{math::Vector3{left, bottom, backward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{left, top, backward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{left, top, forward}, color, math::Vector2{1.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{left, bottom, forward}, color, math::Vector2{1.0f, 1.0f}},
        // Right Face
        BoxComponent::Vertex{math::Vector3{right, bottom, forward}, color, math::Vector2{0.0f, 1.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, forward}, color, math::Vector2{0.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, top, backward}, color, math::Vector2{1.0f, 0.0f}},
        BoxComponent::Vertex{math::Vector3{right, bottom, backward}, color, math::Vector2{1.0f, 1.0f}},
    };
}

std::array<BoxComponent::Index, 36> BoxIndices() {
    // clang-format off
    return {
        // Front face
        0, 1, 2,
        0, 2, 3,
        // Back face
        4, 5, 6,
        4, 6, 7,
        // Top Face
        8, 9, 10,
        8, 10, 11,
        // Bottom Face
        12, 13, 14,
        12, 14, 15,
        // Left Face
        16, 17, 18,
        16, 18, 19,
        // Right Face
        20, 21, 22,
        20, 22, 23,
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
                           const math::Color color, const std::filesystem::path &texture_path,
                           const class Transform &transform, const SceneComponent *parent)
    : TriangleComponent(game, detail::Unmove(detail::BoxVertices(length, height, width, color)),
                        detail::Unmove(detail::BoxIndices()), texture_path, false, transform, parent),
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
