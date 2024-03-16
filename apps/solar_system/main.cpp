#include "game.hpp"

int main() {
    borov_engine::Window window{"Solar system", 800, 800};
    borov_engine::Input input{window};
    Game game{window, input};

    game.Run();
    return 0;
}
