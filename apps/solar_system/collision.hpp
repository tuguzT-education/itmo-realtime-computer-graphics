#pragma once

#ifndef SOLAR_SYSTEM_COLLISION_HPP_INCLUDED
#define SOLAR_SYSTEM_COLLISION_HPP_INCLUDED

#include <borov_engine/math.hpp>

class Collision {
  public:
    virtual ~Collision();

    [[nodiscard]] virtual borov_engine::math::AxisAlignedBox BoxCollision() const;
};

#endif  // SOLAR_SYSTEM_COLLISION_HPP_INCLUDED
