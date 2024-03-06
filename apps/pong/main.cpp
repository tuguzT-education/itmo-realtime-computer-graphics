#include "game.hpp"
#include "player.hpp"

int main() {
    borov_engine::Window window{"Pong", 800, 800};
    borov_engine::Input input{window};

    Game game{window, input};
    game.AddComponent<Player>(
        Direction::Left,
        Player::ControlKeys{
            .up = borov_engine::InputKey::W,
            .down = borov_engine::InputKey::S,
        }
    );
    game.AddComponent<Player>(
        Direction::Right,
        Player::ControlKeys{
            .up = borov_engine::InputKey::Up,
            .down = borov_engine::InputKey::Down,
        }
    );

    game.Run();
    return 0;
}
