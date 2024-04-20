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

using ContainmentType = DirectX::ContainmentType;
using PlaneIntersectionType = DirectX::PlaneIntersectionType;

using Box = DirectX::BoundingOrientedBox;
using AxisAlignedBox = DirectX::BoundingBox;
using Sphere = DirectX::BoundingSphere;
using Frustum = DirectX::BoundingFrustum;

}  // namespace borov_engine::math

#endif  // BOROV_ENGINE_MATH_HPP_INCLUDED
