#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <array>
#include <memory>
#include <vector>

#include "camera_manager.hpp"
#include "concepts.hpp"
#include "debug_draw.hpp"
#include "detail/d3d_ptr.hpp"
#include "input.hpp"
#include "texture_draw.hpp"
#include "timer.hpp"
#include "viewport_manager.hpp"

namespace borov_engine {

class DirectionalLightComponent;
class PointLightComponent;
class SpotLightComponent;

template <typename Range>
concept ComponentRange = RefWrapperRange<Range, Component>;

template <typename Range>
concept ConstComponentRange = RefWrapperRange<Range, const Component>;

class Game {
  public:
    static constexpr std::uint16_t shadow_map_resolution = 2048;
    static constexpr std::string_view shadow_map_resolution_name = "SHADOW_MAP_RESOLUTION";

    static constexpr std::uint8_t shadow_map_cascade_count = 4;
    static constexpr std::string_view shadow_map_cascade_count_name = "SHADOW_MAP_CASCADE_COUNT";

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
    void RemoveCameraManager();

    [[nodiscard]] const ViewportManager &ViewportManager() const;
    [[nodiscard]] class ViewportManager &ViewportManager();

    template <std::derived_from<class ViewportManager> T, typename... Args>
    T &ViewportManager(Args &&...args);

    [[nodiscard]] const DebugDraw &DebugDraw() const;
    [[nodiscard]] class DebugDraw &DebugDraw();

    template <std::derived_from<class DebugDraw> T, typename... Args>
    T &DebugDraw(Args &&...args);

    [[nodiscard]] const TextureDraw &TextureDraw() const;
    [[nodiscard]] class TextureDraw &TextureDraw();

    template <std::derived_from<class TextureDraw> T, typename... Args>
    T &TextureDraw(Args &&...args);

    [[nodiscard]] const DirectionalLightComponent &DirectionalLight() const;
    [[nodiscard]] DirectionalLightComponent &DirectionalLight();

    [[nodiscard]] const PointLightComponent &PointLight() const;
    [[nodiscard]] PointLightComponent &PointLight();

    [[nodiscard]] const SpotLightComponent &SpotLight() const;
    [[nodiscard]] SpotLightComponent &SpotLight();

    [[nodiscard]] const Camera *MainCamera() const;
    [[nodiscard]] Camera *MainCamera();

    [[nodiscard]] std::uint32_t TargetWidth() const;
    [[nodiscard]] std::uint32_t TargetHeight() const;

    [[nodiscard]] math::Vector3 ScreenToWorld(math::Point screen_point, float depth = 0.0f) const;
    [[nodiscard]] math::Point WorldToScreen(math::Vector3 position, const Viewport *viewport = nullptr) const;

    [[nodiscard]] const Timer &Timer() const;

    [[nodiscard]] const Window *Window() const;
    [[nodiscard]] class Window *Window();

    [[nodiscard]] const Input *Input() const;
    [[nodiscard]] class Input *Input();

    [[nodiscard]] bool IsRunning() const;

    template <std::derived_from<Component> T, typename... Args>
    T &AddComponent(Args &&...args);

    [[nodiscard]] ConstComponentRange auto Components() const;
    [[nodiscard]] ComponentRange auto Components();

    void Run();
    void Exit();

  protected:
    virtual void Update(float delta_time);
    virtual void Draw();
    virtual void Draw(const Camera *camera);
    virtual void OnTargetResize();

  private:
    friend Component;

    struct alignas(16) ShadowMapConstantBuffer {
        std::array<math::Matrix4x4, shadow_map_cascade_count> shadow_map_view_projections{};
        std::array<math::Color, shadow_map_cascade_count> shadow_map_debug_colors{
            math::Color{math::colors::linear::White},
            math::Color{math::colors::linear::White},
            math::Color{math::colors::linear::White},
            math::Color{math::colors::linear::White},
        };
        std::array<float, shadow_map_cascade_count> shadow_map_distances{};
    };

    void InitializeDevice();
    void InitializeSwapChain(const class Window &window);
    void InitializeRenderTargetView();
    void InitializeDepthStencilView();

    void InitializeShadowMapResources();

    void UpdateShadowMapConstantBuffer(const ShadowMapConstantBuffer &data);

    void UpdateInternal(float delta_time);
    void DrawInternal();
    void OnWindowResize(WindowResizeData data);

    class Window &window_;
    class Input &input_;

    std::unique_ptr<class ViewportManager> viewport_manager_;
    std::unique_ptr<class CameraManager> camera_manager_;
    std::unique_ptr<class DebugDraw> debug_draw_;
    std::unique_ptr<class TextureDraw> texture_draw_;

    std::unique_ptr<DirectionalLightComponent> directional_light_;
    std::unique_ptr<PointLightComponent> point_light_;
    std::unique_ptr<SpotLightComponent> spot_light_;
    std::vector<std::unique_ptr<Component>> components_;

    detail::D3DPtr<ID3D11Buffer> shadow_map_constant_buffer_;
    detail::D3DPtr<ID3D11GeometryShader> shadow_map_geometry_shader_;
    detail::D3DPtr<ID3DBlob> shadow_map_geometry_shader_byte_code_;

    detail::D3DPtr<ID3D11SamplerState> shadow_map_sampler_state_;
    detail::D3DPtr<ID3D11ShaderResourceView> shadow_map_shader_resource_view_;
    detail::D3DPtr<ID3D11DepthStencilView> shadow_map_depth_view_;
    detail::D3DPtr<ID3D11Texture2D> shadow_map_;

    class Timer timer_;
    Timer::Duration time_per_update_;
    math::Color clear_color_;
    std::uint32_t target_width_;
    std::uint32_t target_height_;
    bool should_exit_;
    bool is_running_;

    detail::D3DPtr<ID3D11DepthStencilView> depth_stencil_view_;
    detail::D3DPtr<ID3D11DepthStencilState> depth_stencil_state_;
    detail::D3DPtr<ID3D11Texture2D> depth_buffer_;

    detail::D3DPtr<ID3D11RenderTargetView> render_target_view_;
    detail::D3DPtr<IDXGISwapChain> swap_chain_;
    detail::D3DPtr<ID3D11DeviceContext> device_context_;
    detail::D3DPtr<ID3D11Device> device_;
};

}  // namespace borov_engine

#include "game.inl"

#endif  // BOROV_ENGINE_GAME_HPP_INCLUDED
