#include "borov_engine/camera.hpp"

namespace borov_engine {

namespace detail {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

}  // namespace detail

auto Camera::Initializer::ProjectionType(const CameraProjectionType projection_type) -> Initializer& {
    this->projection_type = projection_type;
    return *this;
}

auto Camera::Initializer::Width(const float width) -> Initializer& {
    if (width >= 0.0f) {
        this->width = width;
    }
    return *this;
}

auto Camera::Initializer::Height(const float height) -> Initializer& {
    if (height >= 0.0f) {
        this->height = height;
    }
    return *this;
}

auto Camera::Initializer::NearPlane(const float near_plane) -> Initializer& {
    if (near_plane > 0.0f && near_plane != this->far_plane) {
        this->near_plane = near_plane;
    }
    return *this;
}

auto Camera::Initializer::FarPlane(const float far_plane) -> Initializer& {
    if (far_plane > 0.0f && this->near_plane != far_plane) {
        this->far_plane = far_plane;
    }
    return *this;
}

Camera::Camera(class Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer),
      width_{initializer.width},
      height_{initializer.height},
      near_plane_{initializer.near_plane},
      far_plane_{initializer.far_plane},
      projection_type_{initializer.projection_type} {}

const CameraProjectionType& Camera::ProjectionType() const {
    return projection_type_;
}

CameraProjectionType& Camera::ProjectionType() {
    return projection_type_;
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

math::Matrix4x4 Camera::View() const {
    const class Transform world_transform = WorldTransform();
    const math::Vector3 eye = world_transform.position;
    const math::Vector3 target = world_transform.position + world_transform.Forward();
    const math::Vector3 up = world_transform.Up();

    return math::Matrix4x4::CreateLookAt(eye, target, up);
}

math::Matrix4x4 Camera::Projection() const {
    auto perspective = [this](const PerspectiveCameraProjectionType& projection_type) {
        const float fov = projection_type.horizontal_fov;
        return math::Matrix4x4::CreatePerspectiveFieldOfView(fov, AspectRatio(), near_plane_, far_plane_);
    };
    auto orthographic = [this](const OrthographicCameraProjectionType& projection_type) {
        const float units = projection_type.orthographic_units;
        return math::Matrix4x4::CreateOrthographic(units, units * InverseAspectRatio(), near_plane_, far_plane_);
    };
    return std::visit(detail::overloaded{perspective, orthographic}, projection_type_);
}

math::Frustum Camera::Frustum() const {
    const auto [origin, orientation, scale] = WorldTransform();
    const float right_slope = width_ / 2.0f;
    const float left_slope = -width_ / 2.0f;
    const float top_slope = height_ / 2.0f;
    const float bottom_slope = -height_ / 2.0f;
    return math::Frustum{
        origin, orientation, right_slope, left_slope, top_slope, bottom_slope, near_plane_, far_plane_,
    };
}

}  // namespace borov_engine
