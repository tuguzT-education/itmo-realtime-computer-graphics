#pragma once

#ifndef BOROV_ENGINE_CAMERA_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_HPP_INCLUDED

#include "component.hpp"

namespace borov_engine {

enum class CameraProjectionType : std::uint8_t {
    Perspective,
    Orthographic,
};

class Camera : public Component {
  public:
    explicit Camera(Game &game);

    [[nodiscard]] const math::Vector3 &Position() const;
    [[nodiscard]] math::Vector3 &Position();

    [[nodiscard]] const CameraProjectionType &ProjectionType() const;
    [[nodiscard]] CameraProjectionType &ProjectionType();

    [[nodiscard]] math::Vector3 Forward() const;
    [[nodiscard]] math::Vector3 Up() const;
    [[nodiscard]] math::Vector3 Right() const;

    [[nodiscard]] math::Matrix4x4 Rotation() const;
    [[nodiscard]] math::Vector3 EulerRotation() const;
    [[nodiscard]] math::Quaternion QuaternionRotation() const;

    void Rotation(const math::Matrix4x4 &rotation_matrix);
    void Rotation(float yaw, float pitch, float roll);
    void Rotation(const math::Vector3 &euler_angles);
    void Rotation(const math::Quaternion &quaternion);

    [[nodiscard]] float NearPlane() const;
    bool NearPlane(float near_plane);

    [[nodiscard]] float FarPlane() const;
    bool FarPlane(float far_plane);

    [[nodiscard]] float AspectRatio() const;
    [[nodiscard]] float InverseAspectRatio() const;

    [[nodiscard]] float HorizontalFOV() const;
    bool HorizontalFOV(float horizontal_fov);

    [[nodiscard]] float VerticalFOV() const;
    bool VerticalFOV(float vertical_fov);

    [[nodiscard]] float Zoom() const;
    bool Zoom(float zoom);

    void Rotate(const math::Matrix4x4 &rotation_matrix);
    void Rotate(float yaw, float pitch, float roll);
    void Rotate(const math::Vector3 &euler_angles);
    void Rotate(const math::Quaternion &quaternion);

    [[nodiscard]] math::Matrix4x4 View() const;
    [[nodiscard]] math::Matrix4x4 Projection() const;

    [[nodiscard]] math::Matrix4x4 Perspective() const;
    [[nodiscard]] math::Matrix4x4 Orthographic() const;

    void Update(float delta_time) override;
    void Draw() override;

  private:
    math::Vector3 position_;
    math::Vector3 forward_;
    math::Vector3 up_;
    float near_plane_;
    float far_plane_;
    float horizontal_fov_;
    float zoom_;
    CameraProjectionType projection_type_;
};

}

#endif //BOROV_ENGINE_CAMERA_HPP_INCLUDED
