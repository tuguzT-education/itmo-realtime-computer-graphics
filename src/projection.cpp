#include "borov_engine/projection.hpp"

namespace borov_engine {

Projection::~Projection() = default;

math::Frustum Projection::ToFrustum(const Transform& transform, const float width, const float height,
                                    const float near_plane, const float far_plane) const {
    const math::Matrix4x4 projection = ToMatrix(width, height, near_plane, far_plane);

    math::Frustum frustum{projection, true};
    frustum.Transform(frustum, transform.ToMatrix());

    return frustum;
}

OrthographicProjection::OrthographicProjection(const math::Vector2 units) : units_{2.0f} {
    Units(units);
}

math::Vector2 OrthographicProjection::Units() const {
    return units_;
}

bool OrthographicProjection::Units(const math::Vector2 units) {
    constexpr float infinity = std::numeric_limits<float>::infinity();
    if (const math::Vector2 bounds{0.0f, infinity}; units.InBounds(bounds)) {
        return false;
    }

    units_ = units;
    return true;
}

math::Matrix4x4 OrthographicProjection::ToMatrix(const float width, const float height, const float near_plane,
                                                 const float far_plane) const {
    const float inverse_aspect_ratio = (width != 0.0f) ? height / width : 0.0f;
    return math::Matrix4x4::CreateOrthographic(units_.x, units_.y * inverse_aspect_ratio, near_plane, far_plane);
}

PerspectiveProjection::PerspectiveProjection(const float horizontal_fov)
    : horizontal_fov_{horizontal_fov > 0.0f ? horizontal_fov : std::numbers::pi_v<float> / 2.0f} {}

float PerspectiveProjection::HorizontalFOV() const {
    return horizontal_fov_;
}

bool PerspectiveProjection::HorizontalFOV(const float horizontal_fov) {
    if (horizontal_fov_ <= 0.0f) {
        return false;
    }
    horizontal_fov_ = horizontal_fov;
    return true;
}

math::Matrix4x4 PerspectiveProjection::ToMatrix(const float width, const float height, const float near_plane,
                                                const float far_plane) const {
    const float aspect_ratio = (height != 0.0f) ? width / height : 0.0f;
    return math::Matrix4x4::CreatePerspectiveFieldOfView(horizontal_fov_, aspect_ratio, near_plane, far_plane);
}

}  // namespace borov_engine
