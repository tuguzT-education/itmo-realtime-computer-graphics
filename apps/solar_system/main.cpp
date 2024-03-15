#include <borov_engine/box_component.hpp>

#include "game.hpp"

int main() {
    borov_engine::Window window{"Solar system", 800, 800};
    borov_engine::Input input{window};
    Game game{window, input};

    game.AddComponent<borov_engine::BoxComponent>(1.0f, 1.0f, 1.0f);

    game.Run();
    return 0;
}
