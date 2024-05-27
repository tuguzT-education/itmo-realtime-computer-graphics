#pragma once

#ifndef BOROV_ENGINE_CAMERA_HPP_INCLUDED
#define BOROV_ENGINE_CAMERA_HPP_INCLUDED

#include "projection.hpp"
#include "scene_component.hpp"

namespace borov_engine {

using UniqueProjection = std::unique_ptr<Projection>;

class Camera : public SceneComponent {
  public:
    struct Initializer : SceneComponent::Initializer {
        UniqueProjection projection;
        float width = 0.0f;
        float height = 0.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;
    };

    explicit Camera(class Game &game);
    explicit Camera(class Game &game, Initializer &&initializer);

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

    [[nodiscard]] const Projection &Projection() const;
    [[nodiscard]] class Projection &Projection();
    void Projection(UniqueProjection &&projection);

    [[nodiscard]] math::Matrix4x4 ViewMatrix() const;
    [[nodiscard]] math::Matrix4x4 ProjectionMatrix() const;

    [[nodiscard]] math::Frustum Frustum() const;

  private:
    float width_;
    float height_;
    float near_plane_;
    float far_plane_;
    UniqueProjection projection_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_CAMERA_HPP_INCLUDED
