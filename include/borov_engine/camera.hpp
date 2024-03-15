#pragma once

#ifndef BOROV_ENGINE_CAMERA_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_HPP_INCLUDED

#include "math.hpp"

namespace borov_engine {

enum class CameraProjectionType : std::uint8_t {
    Perspective,
    Orthographic,
};

class Camera final {
  public:
    explicit Camera();

    [[nodiscard]] const math::Vector3 &Position() const;
    [[nodiscard]] math::Vector3 &Position();

    [[nodiscard]] const math::Quaternion &Rotation() const;
    [[nodiscard]] math::Quaternion &Rotation();

    [[nodiscard]] const CameraProjectionType &ProjectionType() const;
    [[nodiscard]] CameraProjectionType &ProjectionType();

    [[nodiscard]] math::Vector3 Forward() const;
    [[nodiscard]] math::Vector3 Up() const;
    [[nodiscard]] math::Vector3 Right() const;

    [[nodiscard]] float Width() const;
    bool Width(float width);

    [[nodiscard]] float Height() const;
    bool Height(float height);

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

    [[nodiscard]] float OrthographicUnits() const;
    bool OrthographicUnits(float orthographic_units);

    [[nodiscard]] math::Matrix4x4 View() const;
    [[nodiscard]] math::Matrix4x4 Projection() const;

    [[nodiscard]] math::Matrix4x4 Perspective() const;
    [[nodiscard]] math::Matrix4x4 Orthographic() const;

  private:
    math::Vector3 position_;
    math::Quaternion rotation_;
    float width_;
    float height_;
    float near_plane_;
    float far_plane_;
    float horizontal_fov_;
    float orthographic_units_;
    CameraProjectionType projection_type_;
};

}

#endif //BOROV_ENGINE_CAMERA_HPP_INCLUDED
