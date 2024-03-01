#include <borov_engine/window.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/game.hpp>

int main() {
    borov_engine::Window window{"Solar system", 800, 800};
    borov_engine::Input input{window};
    borov_engine::Game game{window, input};

    game.Run();
    return 0;
}
