#include "borov_engine/camera.hpp"

#include <numbers>

namespace borov_engine {

Camera::Camera(Game &game) :
    Component(game),
    forward_{math::Vector3::Forward},
    up_{math::Vector3::Up},
    near_plane_{0.1f},
    far_plane_{1000.0f},
    horizontal_fov_{std::numbers::pi_v<float> / 2.0f} {}

const math::Vector3 &Camera::Position() const {
    return position_;
}

math::Vector3 &Camera::Position() {
    return position_;
}

math::Matrix4x4 Camera::Rotation() const {
    return math::Matrix4x4::CreateWorld(math::Vector3{}, forward_, up_);
}

math::Vector3 Camera::EulerRotation() const {
    return Rotation().ToEuler();
}

math::Quaternion Camera::QuaternionRotation() const {
    return math::Quaternion::CreateFromRotationMatrix(Rotation());
}

void Camera::Rotation(const math::Matrix4x4 &rotation_matrix) {
    math::Quaternion quaternion = math::Quaternion::CreateFromRotationMatrix(rotation_matrix);
    Rotation(quaternion);
}

void Camera::Rotation(float yaw, float pitch, float roll) {
    math::Quaternion quaternion = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    Rotation(quaternion);
}

void Camera::Rotation(const math::Vector3 &euler_angles) {
    math::Quaternion quaternion = math::Quaternion::CreateFromYawPitchRoll(euler_angles);
    Rotation(quaternion);
}

void Camera::Rotation(const math::Quaternion &quaternion) {
    forward_ = math::Vector3::Transform(math::Vector3::Forward, quaternion);
    up_ = math::Vector3::Transform(math::Vector3::Forward, quaternion);
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
    return width / height;
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
    return 2.0f * std::atan(std::tan(horizontal_fov_ / 2.0f) / AspectRatio());
}

bool Camera::VerticalFOV(float vertical_fov) {
    if (vertical_fov <= 0.0 || vertical_fov >= std::numbers::pi_v<float>) {
        return false;
    }
    horizontal_fov_ = 2.0f * std::atan(std::tan(vertical_fov / 2.0f) * AspectRatio());
    return true;
}

void Camera::Rotate(const math::Matrix4x4 &rotation_matrix) {
    math::Quaternion quaternion = math::Quaternion::CreateFromRotationMatrix(rotation_matrix);
    Rotate(quaternion);
}

void Camera::Rotate(float yaw, float pitch, float roll) {
    math::Quaternion quaternion = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    Rotate(quaternion);
}

void Camera::Rotate(const math::Vector3 &euler_angles) {
    math::Quaternion quaternion = math::Quaternion::CreateFromYawPitchRoll(euler_angles);
    Rotate(quaternion);
}

void Camera::Rotate(const math::Quaternion &quaternion) {
    forward_ = math::Vector3::Transform(forward_, quaternion);
    up_ = math::Vector3::Transform(up_, quaternion);
}

math::Matrix4x4 Camera::View() const {
    return math::Matrix4x4::CreateWorld(position_, forward_, up_);
}

math::Matrix4x4 Camera::Projection(ProjectionType type) const {
    switch (type) {
        case ProjectionType::Perspective: {
            return Perspective();
        }
        case ProjectionType::Orthographic: {
            return Orthographic();
        }
        default: {
            return math::Matrix4x4{};
        }
    }
}

math::Matrix4x4 Camera::Perspective() const {
    return math::Matrix4x4::CreatePerspectiveFieldOfView(horizontal_fov_, AspectRatio(), near_plane_, far_plane_);
}

math::Matrix4x4 Camera::Orthographic() const {
    auto width = static_cast<float>(TargetWidth());
    auto height = static_cast<float>(TargetHeight());
    return math::Matrix4x4::CreateOrthographic(width, height, near_plane_, far_plane_);
}

void Camera::Update(float delta_time) {}

void Camera::Draw() {}

}
