#include "game.hpp"

#include <iostream>

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input), initial_title_{window.Title()} {
    AmbientLight().Primitive().color = borov_engine::math::colors::linear::White;
    input.OnInputKeyDown().AddRaw(this, &Game::OnKeyDown);
}

Game::~Game() {
    const auto input = Input();
    if (input == nullptr) {
        return;
    }

    input->OnInputKeyDown().RemoveByOwner(this);
}

void Game::Draw() {
    const float start_time = Timer().StartTime();
    const float red = start_time - std::floor(start_time);
    ClearColor() = borov_engine::math::Color{red, 0.1f, 0.1f, 1.0f};

    const auto window = Window();
    if (float fps = Timer().FramesPerSecond(); window != nullptr && fps > 0) {
        std::string title = initial_title_;
        std::format_to(std::back_inserter(title), " (FPS: {})", fps);
        window->Title(title);
    }

    borov_engine::Game::Draw();
}

void Game::OnKeyDown(borov_engine::InputKey key) {
    std::cout << "Key: " << static_cast<std::uint16_t>(key) << std::endl;
    if (key == borov_engine::InputKey::Escape) {
        Exit();
    }
}
