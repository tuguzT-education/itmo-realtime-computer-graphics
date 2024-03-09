#include "borov_engine/transform.hpp"

namespace borov_engine {

math::Matrix4x4 Transform::World() const {
    return math::Matrix4x4::CreateTranslation(position)
        * math::Matrix4x4::CreateFromQuaternion(rotation)
        * math::Matrix4x4::CreateScale(scale);
}

}
