#include "borov_engine/viewport_manager.hpp"

#include "borov_engine/game.hpp"

namespace borov_engine {

ViewportManager::ViewportManager(borov_engine::Game &game) : Component(game) {}

const Viewport &ViewportManager::TargetViewport() const {
    return target_viewport_;
}

std::span<const Viewport> ViewportManager::Viewports() const {
    if (split_viewports_.empty()) {
        return {&target_viewport_, 1};
    }
    return split_viewports_;
}

void ViewportManager::Update(float delta_time) {
    for (auto &viewport : split_viewports_) {
        viewport.camera = Game().MainCamera();
    }
    target_viewport_.camera = Game().MainCamera();
}

void ViewportManager::OnTargetResize() {
    target_viewport_.width = static_cast<float>(Game().TargetWidth());
    target_viewport_.height = static_cast<float>(Game().TargetHeight());

    split_viewports_.clear();
    SplitViewports(split_viewports_);
}

void ViewportManager::SplitViewports(std::vector<Viewport> &into) {}

}  // namespace borov_engine
