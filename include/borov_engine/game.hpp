#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <d3d11.h>

#include <vector>
#include <memory>
#include <ranges>
#include <concepts>

#include "detail/d3d_ptr.hpp"
#include "window.hpp"
#include "input.hpp"
#include "timer.hpp"

namespace borov_engine {

class Component;
class Camera;

template<typename View, typename T>
concept RefView = std::ranges::view<View>
    && std::same_as<std::ranges::range_value_t<View>, std::reference_wrapper<T>>;

template<typename T>
concept ComponentView = RefView<T, Component>;

template<typename T>
concept ConstComponentView = RefView<T, const Component>;

class Game {
  public:
    explicit Game(Window &window, Input &input);
    ~Game();

    [[nodiscard]] const Timer::Duration &TimePerUpdate() const;
    [[nodiscard]] Timer::Duration &TimePerUpdate();

    [[nodiscard]] const math::Color &ClearColor() const;
    [[nodiscard]] math::Color &ClearColor();

    [[nodiscard]] const borov_engine::Timer &Timer() const;
    [[nodiscard]] const borov_engine::Window *Window() const;
    [[nodiscard]] const borov_engine::Input *Input() const;

    [[nodiscard]] bool IsRunning() const;

    template<typename T, typename ...Args>
    T &AddComponent(Args &&... args);

    [[nodiscard]] ConstComponentView auto Components() const;
    [[nodiscard]] ComponentView auto Components();

    void Run();
    void Exit();

  protected:
    [[nodiscard]] borov_engine::Window *Window();
    [[nodiscard]] borov_engine::Input *Input();

    virtual void Update(float delta_time);
    virtual void Draw();

  private:
    friend class Component;

    void InitializeDevice();
    void InitializeSwapChain(const borov_engine::Window &window);
    void InitializeRenderTargetView();
    void DrawInternal();

    borov_engine::Window &window_;
    borov_engine::Input &input_;
    Camera *camera_;
    std::vector<std::unique_ptr<Component>> components_;

    borov_engine::Timer timer_;
    Timer::Duration time_per_update_;
    math::Color clear_color_;
    UINT target_width_;
    UINT target_height_;
    bool should_exit_;
    bool is_running_;

    detail::D3DPtr<ID3D11RenderTargetView> render_target_view_;
    detail::D3DPtr<IDXGISwapChain> swap_chain_;
    detail::D3DPtr<ID3D11DeviceContext> device_context_;
    detail::D3DPtr<ID3D11Device> device_;
};

}

#include "game.inl"

#endif //BOROV_ENGINE_GAME_HPP_INCLUDED
