#include "borov_engine/transform.hpp"

namespace borov_engine {

math::Matrix4x4 Transform::World() const {
    return math::Matrix4x4::CreateScale(scale) * math::Matrix4x4::CreateFromQuaternion(rotation) *
           math::Matrix4x4::CreateTranslation(position);
}

void Transform::RotateAround(const math::Vector3 &point, const math::Quaternion &rotate_by) {
    position = math::RotateAround(position, point, rotate_by);
}

}  // namespace borov_engine
