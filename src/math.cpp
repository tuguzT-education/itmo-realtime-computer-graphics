#include "borov_engine/math.hpp"

namespace borov_engine::math {

void RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by,
                  Vector3 &result) {
    result = Vector3::Transform(position - rotation_point, rotate_by) + rotation_point;
}

Vector3 RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by) {
    Vector3 result;
    RotateAround(position, rotation_point, rotate_by, result);
    return result;
}

}  // namespace borov_engine::math
