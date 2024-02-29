#include <borov_engine/window.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/game.hpp>

#include "ball.hpp"

int main() {
    borov_engine::Window window{"Pong", 800, 800};
    borov_engine::Input input{window};
    borov_engine::Game game{window, input};

    auto exit_on_escape_key = [&](const auto key) {
        if (key == borov_engine::InputKey::Escape) {
            game.Exit();
        }
    };
    input.OnInputKeyDown().AddLambda(exit_on_escape_key);

    game.AddComponent<Ball>();

    game.Run();
    return 0;
}
