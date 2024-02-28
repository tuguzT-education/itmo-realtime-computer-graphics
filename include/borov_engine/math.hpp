#pragma once

#ifndef BOROV_ENGINE_MATH_HPP_INCLUDED
#define BOROV_ENGINE_MATH_HPP_INCLUDED

#include <SimpleMath.h>

namespace borov_engine::math {

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Color = DirectX::SimpleMath::Color;

using Matrix4x4 = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

using Viewport = DirectX::SimpleMath::Viewport;
using Plane = DirectX::SimpleMath::Plane;
using Ray = DirectX::SimpleMath::Ray;
using Rectangle = DirectX::SimpleMath::Rectangle;

}

#endif //BOROV_ENGINE_MATH_HPP_INCLUDED
