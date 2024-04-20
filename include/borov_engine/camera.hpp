#pragma once

#ifndef BOROV_ENGINE_CAMERA_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_HPP_INCLUDED

#include <numbers>
#include <variant>

#include "scene_component.hpp"

namespace borov_engine {

struct PerspectiveCameraProjectionType {
    float horizontal_fov = std::numbers::pi_v<float> / 2.0f;
};

struct OrthographicCameraProjectionType {
    float orthographic_units = 2.0f;
};

using CameraProjectionType = std::variant<PerspectiveCameraProjectionType, OrthographicCameraProjectionType>;

class Camera : public SceneComponent {
  public:
    explicit Camera(class Game &game);

    [[nodiscard]] const CameraProjectionType &ProjectionType() const;
    [[nodiscard]] CameraProjectionType &ProjectionType();

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

    [[nodiscard]] math::Matrix4x4 View() const;
    [[nodiscard]] math::Matrix4x4 Projection() const;

    [[nodiscard]] math::Frustum Frustum() const;

  private:
    float width_;
    float height_;
    float near_plane_;
    float far_plane_;
    CameraProjectionType projection_type_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_CAMERA_HPP_INCLUDED
