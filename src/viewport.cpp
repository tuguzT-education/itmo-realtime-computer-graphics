#include "borov_engine/viewport.hpp"

namespace borov_engine {

Viewport::Viewport(Camera *camera) noexcept: math::Viewport(), camera{camera} {}

Viewport::Viewport(float x,
                   float y,
                   float width,
                   float height,
                   float min_depth,
                   float max_depth,
                   Camera *camera) noexcept
    : math::Viewport(x, y, width, height, min_depth, max_depth),
      camera{camera} {}

Viewport::Viewport(const RECT &rect, Camera *camera) noexcept: math::Viewport(rect), camera{camera} {}

}
