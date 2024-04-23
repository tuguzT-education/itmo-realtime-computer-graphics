#include "collision.hpp"

Collision::~Collision() = default;

borov_engine::math::AxisAlignedBox Collision::BoxCollision() const {
    return borov_engine::math::AxisAlignedBox{};
}
