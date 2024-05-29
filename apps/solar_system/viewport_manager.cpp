#include "viewport_manager.hpp"

#include <borov_engine/camera.hpp>

ViewportManager::ViewportManager(borov_engine::Game &game) : borov_engine::ViewportManager(game) {}

void ViewportManager::SplitViewports(std::vector<borov_engine::Viewport> &into) {
    // const borov_engine::Viewport &target_viewport = TargetViewport();
    //
    // target_viewport.camera->Width(target_viewport.width / 2.0f);
    // into.emplace_back(target_viewport.x, target_viewport.y, target_viewport.width / 2.0f, target_viewport.height,
    //                   target_viewport.minDepth, target_viewport.maxDepth, target_viewport.camera);
    // into.emplace_back(target_viewport.x + target_viewport.width / 2.0f, target_viewport.y, target_viewport.width
    // / 2.0f,
    //                   target_viewport.height, target_viewport.minDepth, target_viewport.maxDepth,
    //                   target_viewport.camera);
}
