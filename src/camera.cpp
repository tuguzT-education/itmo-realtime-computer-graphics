#include "borov_engine/camera.hpp"

#include <numbers>

namespace borov_engine {

Camera::Camera(Game &game) :
    Component(game),
    position_{math::Vector3::Backward},
    rotation_{math::Quaternion::Identity},
    near_plane_{0.1f},
    far_plane_{1000.0f},
    horizontal_fov_{std::numbers::pi_v<float> / 2.0f},
    orthographic_units_{2.0f},
    projection_type_{CameraProjectionType::Orthographic} {}

const math::Vector3 &Camera::Position() const {
    return position_;
}

math::Vector3 &Camera::Position() {
    return position_;
}

const math::Quaternion &Camera::Rotation() const {
    return rotation_;
}

math::Quaternion &Camera::Rotation() {
    return rotation_;
}

const CameraProjectionType &Camera::ProjectionType() const {
    return projection_type_;
}

CameraProjectionType &Camera::ProjectionType() {
    return projection_type_;
}

math::Vector3 Camera::Forward() const {
    return math::Vector3::Transform(math::Vector3::Forward, rotation_);
}

math::Vector3 Camera::Up() const {
    return math::Vector3::Transform(math::Vector3::Up, rotation_);
}

math::Vector3 Camera::Right() const {
    return math::Vector3::Transform(math::Vector3::Right, rotation_);
}

float Camera::NearPlane() const {
    return near_plane_;
}

bool Camera::NearPlane(float near_plane) {
    if (near_plane <= 0.0f || near_plane == far_plane_) {
        return false;
    }
    near_plane_ = near_plane;
    return true;
}

float Camera::FarPlane() const {
    return far_plane_;
}

bool Camera::FarPlane(float far_plane) {
    if (far_plane <= 0.0f || near_plane_ == far_plane) {
        return false;
    }
    far_plane_ = far_plane;
    return true;
}

float Camera::AspectRatio() const {
    auto width = static_cast<float>(TargetWidth());
    auto height = static_cast<float>(TargetHeight());
    return (height != 0.0f) ? (width / height) : 0.0f;
}

float Camera::InverseAspectRatio() const {
    float aspect_ratio = AspectRatio();
    return (aspect_ratio != 0.0f) ? (1.0f / aspect_ratio) : 0.0f;
}

float Camera::HorizontalFOV() const {
    return horizontal_fov_;
}

bool Camera::HorizontalFOV(float horizontal_fov) {
    if (horizontal_fov <= 0.0 || horizontal_fov >= std::numbers::pi_v<float>) {
        return false;
    }
    horizontal_fov_ = horizontal_fov;
    return true;
}

float Camera::VerticalFOV() const {
    return 2.0f * std::atan(std::tan(horizontal_fov_ / 2.0f) * InverseAspectRatio());
}

bool Camera::VerticalFOV(float vertical_fov) {
    if (vertical_fov <= 0.0 || vertical_fov >= std::numbers::pi_v<float>) {
        return false;
    }
    horizontal_fov_ = 2.0f * std::atan(std::tan(vertical_fov / 2.0f) * AspectRatio());
    return true;
}

float Camera::OrthographicUnits() const {
    return orthographic_units_;
}

bool Camera::OrthographicUnits(float orthographic_units) {
    if (orthographic_units <= 0.0) {
        return false;
    }
    orthographic_units_ = orthographic_units;
    return true;
}

math::Matrix4x4 Camera::View() const {
    math::Vector3 eye = position_;
    math::Vector3 target = position_ + Forward();
    math::Vector3 up = Up();
    return math::Matrix4x4::CreateLookAt(eye, target, up);
}

math::Matrix4x4 Camera::Projection() const {
    switch (projection_type_) {
        case CameraProjectionType::Perspective: {
            return Perspective();
        }
        case CameraProjectionType::Orthographic: {
            return Orthographic();
        }
        default: {
            return math::Matrix4x4::Identity;
        }
    }
}

math::Matrix4x4 Camera::Perspective() const {
    return math::Matrix4x4::CreatePerspectiveFieldOfView(horizontal_fov_,
                                                         AspectRatio(),
                                                         near_plane_,
                                                         far_plane_);
}

math::Matrix4x4 Camera::Orthographic() const {
    return math::Matrix4x4::CreateOrthographic(orthographic_units_,
                                               orthographic_units_ * InverseAspectRatio(),
                                               near_plane_,
                                               far_plane_);
}

void Camera::Update(float delta_time) {}

void Camera::Draw() {}

}
