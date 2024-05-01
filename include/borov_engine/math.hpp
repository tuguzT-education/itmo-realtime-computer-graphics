#pragma once

#ifndef BOROV_ENGINE_MATH_HPP_INCLUDED
#define BOROV_ENGINE_MATH_HPP_INCLUDED

#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <SimpleMath.h>

namespace borov_engine::math {

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;

void Normalize(const Vector3 &vector, Vector3 &result);
Vector3 Normalize(const Vector3 &vector);

using Color = DirectX::SimpleMath::Color;

namespace colors {

namespace srgb = DirectX::Colors;
namespace linear = DirectX::ColorsLinear;

}  // namespace colors

using Matrix4x4 = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

void RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by, Vector3 &result);
Vector3 RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by);

using Viewport = DirectX::SimpleMath::Viewport;
using Plane = DirectX::SimpleMath::Plane;
using Ray = DirectX::SimpleMath::Ray;
using Rectangle = DirectX::SimpleMath::Rectangle;

bool Intersects(Plane lhs, Plane rhs);

struct Point {
    std::int32_t x;
    std::int32_t y;
};

using ContainmentType = DirectX::ContainmentType;
using PlaneIntersectionType = DirectX::PlaneIntersectionType;

using Box = DirectX::BoundingOrientedBox;
using AxisAlignedBox = DirectX::BoundingBox;
using Sphere = DirectX::BoundingSphere;
using Frustum = DirectX::BoundingFrustum;

struct Triangle {
    Vector3 point0;
    Vector3 point1;
    Vector3 point2;

    [[nodiscard]] Vector3 Tangent() const;
    [[nodiscard]] Vector3 Normal() const;
    [[nodiscard]] Vector3 Binormal() const;

    [[nodiscard]] Plane Plane() const;

    [[nodiscard]] bool Intersects(const AxisAlignedBox &axis_aligned_box) const;
    [[nodiscard]] bool Intersects(const Box &box) const;
    [[nodiscard]] bool Intersects(Sphere sphere) const;
    [[nodiscard]] bool Intersects(const Frustum &frustum) const;
    [[nodiscard]] bool Intersects(const Ray &ray, float &dist) const;
    [[nodiscard]] bool Intersects(const Triangle &other) const;
    [[nodiscard]] PlaneIntersectionType Intersects(math::Plane plane) const;

    [[nodiscard]] ContainmentType ContainedBy(const AxisAlignedBox &axis_aligned_box) const;
    [[nodiscard]] ContainmentType ContainedBy(const Box &box) const;
    [[nodiscard]] ContainmentType ContainedBy(Sphere sphere) const;
    [[nodiscard]] ContainmentType ContainedBy(const Frustum &frustum) const;
    [[nodiscard]] ContainmentType ContainedBy(math::Plane plane0, math::Plane plane1, math::Plane plane2,
                                              math::Plane plane3, math::Plane plane4, math::Plane plane5) const;
};

}  // namespace borov_engine::math

#endif  // BOROV_ENGINE_MATH_HPP_INCLUDED
