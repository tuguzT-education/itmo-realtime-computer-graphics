#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <vector>
#include <memory>
#include <ranges>
#include <concepts>

#include "detail/d3d_ptr.hpp"
#include "window.hpp"
#include "input.hpp"
#include "timer.hpp"
#include "camera_manager.hpp"
#include "viewport_manager.hpp"

namespace borov_engine {

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
    virtual ~Game();

    [[nodiscard]] const Timer::Duration &TimePerUpdate() const;
    [[nodiscard]] Timer::Duration &TimePerUpdate();

    [[nodiscard]] const math::Color &ClearColor() const;
    [[nodiscard]] math::Color &ClearColor();

    [[nodiscard]] const borov_engine::CameraManager *CameraManager() const;
    [[nodiscard]] borov_engine::CameraManager *CameraManager();

    template<std::derived_from<borov_engine::CameraManager> T, typename ...Args>
    T &CameraManager(Args &&... args);

    [[nodiscard]] const borov_engine::ViewportManager &ViewportManager() const;
    [[nodiscard]] borov_engine::ViewportManager &ViewportManager();

    template<std::derived_from<borov_engine::ViewportManager> T, typename ...Args>
    T &ViewportManager(Args &&... args);

    [[nodiscard]] const Camera *MainCamera() const;
    [[nodiscard]] Camera *MainCamera();

    [[nodiscard]] std::uint32_t TargetWidth() const;
    [[nodiscard]] std::uint32_t TargetHeight() const;

    [[nodiscard]] const borov_engine::Timer &Timer() const;

    [[nodiscard]] const borov_engine::Window *Window() const;
    [[nodiscard]] borov_engine::Window *Window();

    [[nodiscard]] const borov_engine::Input *Input() const;
    [[nodiscard]] borov_engine::Input *Input();

    [[nodiscard]] bool IsRunning() const;

    template<std::derived_from<borov_engine::Component> T, typename ...Args>
    T &AddComponent(Args &&... args);

    [[nodiscard]] ConstComponentView auto Components() const;
    [[nodiscard]] ComponentView auto Components();

    void Run();
    void Exit();

  protected:
    virtual void Update(float delta_time);
    virtual void Draw();
    virtual void OnTargetResize();

  private:
    friend class Component;

    void InitializeDevice();
    void InitializeSwapChain(const borov_engine::Window &window);
    void InitializeRenderTargetView();

    void DrawInternal();
    void OnWindowResize(WindowResizeData data);
    void ViewportManagerPostInit();

    borov_engine::Window &window_;
    borov_engine::Input &input_;
    std::unique_ptr<borov_engine::ViewportManager> viewport_manager_;
    std::unique_ptr<borov_engine::CameraManager> camera_manager_;
    std::vector<std::unique_ptr<borov_engine::Component>> components_;

    borov_engine::Timer timer_;
    Timer::Duration time_per_update_;
    math::Color clear_color_;
    std::uint32_t target_width_;
    std::uint32_t target_height_;
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
