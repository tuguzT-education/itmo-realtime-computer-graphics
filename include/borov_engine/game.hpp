#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <concepts>
#include <memory>
#include <vector>

#include "camera_manager.hpp"
#include "detail/d3d_ptr.hpp"
#include "input.hpp"
#include "timer.hpp"
#include "viewport_manager.hpp"

namespace borov_engine {

template <typename View, typename T>
concept RefView = std::ranges::view<View> && std::same_as<std::ranges::range_value_t<View>, std::reference_wrapper<T>>;

template <typename T>
concept ComponentView = RefView<T, Component>;

template <typename T>
concept ConstComponentView = RefView<T, const Component>;

class Game {
  public:
    explicit Game(Window &window, Input &input);
    virtual ~Game();

    [[nodiscard]] const Timer::Duration &TimePerUpdate() const;
    [[nodiscard]] Timer::Duration &TimePerUpdate();

    [[nodiscard]] const math::Color &ClearColor() const;
    [[nodiscard]] math::Color &ClearColor();

    [[nodiscard]] const CameraManager *CameraManager() const;
    [[nodiscard]] class CameraManager *CameraManager();

    template <std::derived_from<class CameraManager> T, typename... Args>
    T &CameraManager(Args &&...args);

    [[nodiscard]] const ViewportManager &ViewportManager() const;
    [[nodiscard]] class ViewportManager &ViewportManager();

    template <std::derived_from<class ViewportManager> T, typename... Args>
    T &ViewportManager(Args &&...args);

    [[nodiscard]] const Camera *MainCamera() const;
    [[nodiscard]] Camera *MainCamera();

    [[nodiscard]] std::uint32_t TargetWidth() const;
    [[nodiscard]] std::uint32_t TargetHeight() const;

    [[nodiscard]] const Timer &Timer() const;

    [[nodiscard]] const Window *Window() const;
    [[nodiscard]] class Window *Window();

    [[nodiscard]] const Input *Input() const;
    [[nodiscard]] class Input *Input();

    [[nodiscard]] bool IsRunning() const;

    template <std::derived_from<Component> T, typename... Args>
    T &AddComponent(Args &&...args);

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
    void InitializeSwapChain(const class Window &window);
    void InitializeRenderTargetView();

    void DrawInternal();
    void OnWindowResize(WindowResizeData data);

    class Window &window_;
    class Input &input_;
    std::unique_ptr<class ViewportManager> viewport_manager_;
    std::unique_ptr<class CameraManager> camera_manager_;
    std::vector<std::unique_ptr<Component>> components_;

    class Timer timer_;
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

}  // namespace borov_engine

#include "game.inl"

#endif  // BOROV_ENGINE_GAME_HPP_INCLUDED
