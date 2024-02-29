#include <borov_engine/window.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/game.hpp>

#include "ball.hpp"
#include "player.hpp"
#include "referee.hpp"

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
    game.AddComponent<Referee>();

    game.Run();
    return 0;
}
