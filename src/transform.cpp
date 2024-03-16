#include "borov_engine/transform.hpp"

namespace borov_engine {

math::Matrix4x4 Transform::World() const {
    return math::Matrix4x4::CreateTranslation(position)
        * math::Matrix4x4::CreateFromQuaternion(rotation)
        * math::Matrix4x4::CreateScale(scale);
}

void Transform::RotateAround(const math::Vector3 &point, const math::Quaternion &rotate_by) {
    position -= point;
    position = math::Vector3::Transform(position, rotate_by);
    position += point;
}

}
