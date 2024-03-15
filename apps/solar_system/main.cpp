#include <borov_engine/geometric_primitive_component.hpp>

#include "game.hpp"

using namespace borov_engine;

int main() {
    Window window{"Solar system", 800, 800};
    Input input{window};
    ::Game game{window, input};

    game.AddComponent<GeometricPrimitiveComponent>(GeometricPrimitiveType::Cone,
                                                   Transform{.position = math::Vector3::Left});
    game.AddComponent<GeometricPrimitiveComponent>(GeometricPrimitiveType::Sphere,
                                                   Transform{.position = math::Vector3::Zero});
    game.AddComponent<GeometricPrimitiveComponent>(GeometricPrimitiveType::Torus,
                                                   Transform{.position = math::Vector3::Right * 1.25f});

    game.Run();
    return 0;
}
