#include "game.hpp"

#include <array>

#include "camera_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input) : borov_engine::Game(window, input) {
    CameraManager<::CameraManager>();
    SetupViewports();
}

void Game::OnTargetResize() {
    borov_engine::Game::OnTargetResize();
    SetupViewports();
}

void Game::SetupViewports() {
    auto width = static_cast<float>(TargetWidth());
    auto height = static_cast<float>(TargetHeight());

    std::array viewports{
        borov_engine::Viewport{0.0f, 0.0f,
                               width / 2.0f, height,
                               0.0f, 1.0f,
                               MainCamera()},
        borov_engine::Viewport{width / 2.0f, 0.0f,
                               width / 2.0f, height,
                               0.0f, 1.0f,
                               MainCamera()},
    };
    Viewports(viewports);
}
