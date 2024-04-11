#include "game.hpp"
#include "player.hpp"

int main() {
    borov_engine::Window window{"Pong", 800, 800};
    borov_engine::Input input{window};

    Game game{window, input};

    Player::ControlKeys left_player_control_keys{
        .up = borov_engine::InputKey::W,
        .down = borov_engine::InputKey::S,
    };
    game.AddComponent<Player>(Direction::Left, left_player_control_keys);

    Player::ControlKeys right_player_control_keys{
        .up = borov_engine::InputKey::Up,
        .down = borov_engine::InputKey::Down,
    };
    game.AddComponent<Player>(Direction::Right, right_player_control_keys);

    game.Run();
    return 0;
}
