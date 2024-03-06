#include "game.hpp"
#include "square_component.hpp"

int main() {
    borov_engine::Window window{"Example", 800, 800};
    borov_engine::Input input{window};

    Game game{window, input};
    game.AddComponent<SquareComponent>();

    game.Run();
    return 0;
}
