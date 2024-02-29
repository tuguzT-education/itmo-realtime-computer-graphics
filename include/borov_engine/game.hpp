#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <d3d11.h>

#include <vector>
#include <memory>
#include <span>

#include "borov_engine/detail/d3d_ptr.hpp"
#include "window.hpp"
#include "input.hpp"
#include "timer.hpp"

namespace borov_engine {

class Game {
  private:
    friend class Component;

  public:
    explicit Game(Window &window, Input &input);
    ~Game();

    [[nodiscard]] const Timer::Duration &TimePerUpdate() const;
    void TimePerUpdate(Timer::Duration time_per_update);

    [[nodiscard]] const math::Color &ClearColor() const;
    [[nodiscard]] math::Color &ClearColor();

    [[nodiscard]] bool IsRunning() const;

    template<typename T, typename ...Args>
    void AddComponent(Args &&... args);

    [[nodiscard]] std::span<const std::unique_ptr<Component>> Components() const;

    void Run();
    void Exit();

  private:
    void InitializeDevice();
    void InitializeSwapChain(const Window &window);
    void InitializeRenderTargetView();

    void Update(float delta_time);
    void Draw();

    [[nodiscard]] std::span<std::unique_ptr<Component>> Components();

    Timer timer_;
    Window &window_;
    Input &input_;
    Timer::Duration time_per_update_;
    math::Color clear_color_;
    UINT target_width_;
    UINT target_height_;
    bool should_exit_;
    bool is_running_;
    std::vector<std::unique_ptr<Component>> components_;

    detail::D3DPtr<ID3D11RenderTargetView> render_target_view_;
    detail::D3DPtr<IDXGISwapChain> swap_chain_;
    detail::D3DPtr<ID3D11DeviceContext> device_context_;
    detail::D3DPtr<ID3D11Device> device_;
};

}

#include "game.inl"

#endif //BOROV_ENGINE_GAME_HPP_INCLUDED
