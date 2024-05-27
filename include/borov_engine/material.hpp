#pragma once

#ifndef BOROV_ENGINE_MATERIAL_HPP_INCLUDED
#define BOROV_ENGINE_MATERIAL_HPP_INCLUDED

#include "math.hpp"

namespace borov_engine {

struct alignas(16) Material {
    math::Color ambient{math::colors::linear::White};
    math::Color diffuse{math::colors::linear::White};
    math::Color specular{math::colors::linear::White};
    math::Color emissive;
    float exponent = 16.0f;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_MATERIAL_HPP_INCLUDED
