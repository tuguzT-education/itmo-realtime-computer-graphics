#include "game.hpp"

#include <iostream>

Game::Game(borov_engine::Window &window, borov_engine::Input &input) : borov_engine::Game(window, input),
                                                                       initial_title_{window.Title()} {
    auto exit_on_escape_key = [](const auto key) {
        std::cout << "Key: " << static_cast<std::uint16_t>(key) << std::endl;
    };
    input.OnInputKeyDown().AddLambda(exit_on_escape_key);
}

void Game::Update(float delta_time) {
    borov_engine::Game::Update(delta_time);

    auto input = Input();
    if (input != nullptr && input->IsKeyDown(borov_engine::InputKey::Escape)) {
        Exit();
    }
}

void Game::Draw() {
    float start_time = Timer().StartTime();
    float red = start_time - std::floor(start_time);
    ClearColor() = borov_engine::math::Color{red, 0.1f, 0.1f, 1.0f};

    auto window = Window();
    float fps = Timer().FramesPerSecond();
    if (window != nullptr && fps > 0) {
        std::string title = initial_title_;
        std::format_to(std::back_inserter(title), " (FPS: {})", fps);
        window->Title(title);
    }

    borov_engine::Game::Draw();
}
