#include <borov_engine/triangle_component.hpp>

#include "game.hpp"

int main() {
    borov_engine::Window window{"Katamari", 800, 800};
    borov_engine::Input input{window};
    Game game{window, input};

    auto &component = game.AddComponent<borov_engine::TriangleComponent>();
    component.LoadMesh("resources/meshes/hog.fbx");

    game.Run();
    return 0;
}
