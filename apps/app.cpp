#include <borov_engine/window.hpp>
#include <borov_engine/input_device.hpp>
#include <borov_engine/game.hpp>

#include <iostream>

int main() {
    borov_engine::Window window{"Application", 800, 800};
    borov_engine::InputDevice input_device{window};
    borov_engine::Game game{window};

    auto exit_on_escape_key = [&](const auto key) {
        std::cout << "Key: " << static_cast<std::uint16_t>(key) << std::endl;
        if (key == borov_engine::InputKey::Escape) {
            game.Exit();
        }
    };
    input_device.GetOnInputKeyDown().AddLambda(exit_on_escape_key);

    game.Run();
    return 0;
}
