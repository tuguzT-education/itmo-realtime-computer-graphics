#include <borov_engine/window.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/game.hpp>

#include <iostream>

int main() {
    borov_engine::Window window{"Application", 800, 800};
    borov_engine::Input input{window};
    borov_engine::Game game{window, input};

    auto exit_on_escape_key = [&](const auto key) {
        std::cout << "Key: " << static_cast<std::uint16_t>(key) << std::endl;
        if (key == borov_engine::InputKey::Escape) {
            game.Exit();
        }
    };
    input.GetOnInputKeyDown().AddLambda(exit_on_escape_key);

    game.Run();
    return 0;
}
