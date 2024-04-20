#include "borov_engine/transform.hpp"

namespace borov_engine {

math::Matrix4x4 Transform::ToMatrix() const {
    return math::Matrix4x4::CreateScale(scale) * math::Matrix4x4::CreateFromQuaternion(rotation) *
           math::Matrix4x4::CreateTranslation(position);
}

void Transform::Concatenate(const Transform& parent, const Transform& child, Transform& result) {
    result = Transform{
        .position = math::Vector3::Transform(parent.scale * child.position, parent.rotation) + parent.position,
        .rotation = math::Quaternion::Concatenate(parent.rotation, child.rotation),
        .scale = parent.scale * child.scale,
    };
}

Transform Transform::Concatenate(const Transform& parent, const Transform& child) {
    Transform result;
    Concatenate(parent, child, result);
    return result;
}

void Transform::Inverse(const Transform& transform, Transform& result) {
    math::Quaternion inv_rotation;
    transform.rotation.Inverse(inv_rotation);

    const auto invert_scale_axis = [](auto axis) { return axis != 0 ? 1 / axis : 0; };
    const math::Vector3 inv_scale{
        invert_scale_axis(transform.scale.x),
        invert_scale_axis(transform.scale.y),
        invert_scale_axis(transform.scale.z),
    };

    const math::Vector3 inv_position = math::Vector3::Transform(inv_scale * (transform.position * -1), inv_rotation);

    result = Transform{
        .position = inv_position,
        .rotation = inv_rotation,
        .scale = inv_scale,
    };
}

Transform Transform::Inverse(const Transform& transform) {
    Transform result;
    Inverse(transform, result);
    return result;
}

math::Vector3 Transform::Right() const {
    return math::Vector3::Transform(math::Vector3::Right, rotation);
}

math::Vector3 Transform::Up() const {
    return math::Vector3::Transform(math::Vector3::Up, rotation);
}

math::Vector3 Transform::Forward() const {
    return math::Vector3::Transform(math::Vector3::Forward, rotation);
}

void Transform::RotateAround(const math::Vector3& point, const math::Quaternion& rotate_by) {
    position = math::RotateAround(position, point, rotate_by);
}

}  // namespace borov_engine
