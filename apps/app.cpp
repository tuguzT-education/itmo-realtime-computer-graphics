#include <borov_engine/window.hpp>
#include <borov_engine/input.hpp>
#include <borov_engine/game.hpp>
#include <borov_engine/triangle_component.hpp>

#include <array>
#include <iostream>

#include "clear_screen_component.hpp"
#include "window_title_component.hpp"

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
    input.OnInputKeyDown().AddLambda(exit_on_escape_key);

    game.AddComponent<ClearScreenComponent>();
    game.AddComponent<WindowTitleComponent>();

    std::array vertices{
        borov_engine::TriangleComponent::Vertex{
            {0.5f, 0.5f, 0.5f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
        },
        borov_engine::TriangleComponent::Vertex{
            {-0.5f, -0.5f, 0.5f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
        },
        borov_engine::TriangleComponent::Vertex{
            {0.5f, -0.5f, 0.5f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
        },
        borov_engine::TriangleComponent::Vertex{
            {-0.5f, 0.5f, 0.5f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
        },
    };
    std::array indices{0, 1, 2, 1, 0, 3};
    game.AddComponent<borov_engine::TriangleComponent>(vertices, indices);

    game.Run();
    return 0;
}
