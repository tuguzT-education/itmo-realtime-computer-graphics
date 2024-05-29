#include "borov_engine/camera.hpp"

namespace borov_engine {

namespace detail {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

}  // namespace detail

Camera::Camera(class Game& game)
    : SceneComponent(game),
      width_{0.0f},
      height_{0.0f},
      near_plane_{0.1f},
      far_plane_{100.0f},
      projection_{std::make_unique<OrthographicProjection>()} {}

Camera::Camera(class Game& game, Initializer&& initializer)
    : SceneComponent(game, initializer),
      width_{initializer.width},
      height_{initializer.height},
      near_plane_{initializer.near_plane},
      far_plane_{initializer.far_plane},
      projection_{std::move(initializer.projection)} {
    if (projection_ != nullptr) {
        return;
    }
    projection_ = std::make_unique<OrthographicProjection>();
}

const Projection& Camera::Projection() const {
    return *projection_;
}

Projection& Camera::Projection() {
    return *projection_;
}

void Camera::Projection(UniqueProjection&& projection) {
    projection_ = std::move(projection);
}

math::Matrix4x4 Camera::ViewMatrix() const {
    return WorldTransform().ViewMatrix();
}

math::Matrix4x4 Camera::ProjectionMatrix() const {
    return projection_->ToMatrix(width_, height_, near_plane_, far_plane_);
}

float Camera::Width() const {
    return width_;
}

bool Camera::Width(const float width) {
    if (width < 0.0f) {
        return false;
    }
    width_ = width;
    return true;
}

float Camera::Height() const {
    return height_;
}

bool Camera::Height(const float height) {
    if (height < 0.0f) {
        return false;
    }
    height_ = height;
    return true;
}

float Camera::NearPlane() const {
    return near_plane_;
}

bool Camera::NearPlane(const float near_plane) {
    if (near_plane <= 0.0f || near_plane == far_plane_) {
        return false;
    }
    near_plane_ = near_plane;
    return true;
}

float Camera::FarPlane() const {
    return far_plane_;
}

bool Camera::FarPlane(const float far_plane) {
    if (far_plane <= 0.0f || near_plane_ == far_plane) {
        return false;
    }
    far_plane_ = far_plane;
    return true;
}

float Camera::AspectRatio() const {
    return height_ != 0.0f ? width_ / height_ : 0.0f;
}

float Camera::InverseAspectRatio() const {
    const float aspect_ratio = AspectRatio();
    return aspect_ratio != 0.0f ? 1.0f / aspect_ratio : 0.0f;
}

math::Frustum Camera::Frustum() const {
    math::Frustum frustum{ProjectionMatrix(), true};
    frustum.Transform(frustum, WorldTransform().ToMatrix());

    return frustum;
}

}  // namespace borov_engine
