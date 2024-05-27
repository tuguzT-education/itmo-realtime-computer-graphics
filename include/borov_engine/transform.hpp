#pragma once

#ifndef BOROV_ENGINE_TRANSFORM_HPP_INCLUDED
#define BOROV_ENGINE_TRANSFORM_HPP_INCLUDED

#include "math.hpp"

namespace borov_engine {

struct Transform {
    alignas(16) math::Vector3 position;
    alignas(16) math::Quaternion rotation;
    alignas(16) math::Vector3 scale = math::Vector3::One;

    [[nodiscard]] math::Matrix4x4 ToMatrix() const;
    [[nodiscard]] math::Matrix4x4 ViewMatrix() const;

    static void Concatenate(const Transform &parent, const Transform &child, Transform &result);
    static Transform Concatenate(const Transform &parent, const Transform &child);

    static void Inverse(const Transform &transform, Transform &result);
    static Transform Inverse(const Transform &transform);

    [[nodiscard]] math::Vector3 Right() const;
    [[nodiscard]] math::Vector3 Up() const;
    [[nodiscard]] math::Vector3 Forward() const;

    void RotateAround(const math::Vector3 &point, const math::Quaternion &rotate_by);
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_TRANSFORM_HPP_INCLUDED
