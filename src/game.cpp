#include "borov_engine/game.hpp"

#include <d3dcompiler.h>

#include <array>

#include "borov_engine/camera.hpp"
#include "borov_engine/camera_manager.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"
#include "borov_engine/light.hpp"
#include "borov_engine/triangle_component.hpp"
#include "borov_engine/viewport_manager.hpp"

namespace borov_engine {

constexpr Timer::Duration default_time_per_update = std::chrono::microseconds{6500};

Game::Game(class Window &window, class Input &input)
    : window_{window},
      input_{input},
      time_per_update_{default_time_per_update},
      target_width_{},
      target_height_{},
      should_exit_{},
      is_running_{} {
    InitializeDevice();
    InitializeSwapChain(window);
    InitializeRenderTargetView();
    InitializeDepthStencilView();
    InitializeShadowMapResources();

    ViewportManager<class ViewportManager>();
    DebugDraw<class DebugDraw>();
    directional_light_ = std::make_unique<DirectionalLightComponent>(*this);
    directional_light_->IsLightEnabled() = true;
    directional_light_->Ambient() = math::Color{math::colors::linear::White};

    point_light_ = std::make_unique<PointLightComponent>(*this);
    spot_light_ = std::make_unique<SpotLightComponent>(*this);

    window_.OnResize().AddRaw(this, &Game::OnWindowResize);
}

Game::~Game() {
    window_.OnResize().RemoveByOwner(this);
}

const Timer::Duration &Game::TimePerUpdate() const {
    return time_per_update_;
}

Timer::Duration &Game::TimePerUpdate() {
    return time_per_update_;
}

const math::Color &Game::ClearColor() const {
    return clear_color_;
}

math::Color &Game::ClearColor() {
    return clear_color_;
}

const CameraManager *Game::CameraManager() const {
    return camera_manager_.get();
}

CameraManager *Game::CameraManager() {
    return camera_manager_.get();
}

void Game::RemoveCameraManager() {
    camera_manager_ = nullptr;
}

const ViewportManager &Game::ViewportManager() const {
    return *viewport_manager_;
}

ViewportManager &Game::ViewportManager() {
    return *viewport_manager_;
}

const DebugDraw &Game::DebugDraw() const {
    return *debug_draw_;
}

DebugDraw &Game::DebugDraw() {
    return *debug_draw_;
}

const DirectionalLightComponent &Game::DirectionalLight() const {
    return *directional_light_;
}

DirectionalLightComponent &Game::DirectionalLight() {
    return *directional_light_;
}

const PointLightComponent &Game::PointLight() const {
    return *point_light_;
}

PointLightComponent &Game::PointLight() {
    return *point_light_;
}

const SpotLightComponent &Game::SpotLight() const {
    return *spot_light_;
}

SpotLightComponent &Game::SpotLight() {
    return *spot_light_;
}

const Camera *Game::MainCamera() const {
    if (camera_manager_ == nullptr) {
        return nullptr;
    }
    return camera_manager_->MainCamera();
}

Camera *Game::MainCamera() {
    if (camera_manager_ == nullptr) {
        return nullptr;
    }
    return camera_manager_->MainCamera();
}

std::uint32_t Game::TargetWidth() const {
    return target_width_;
}

std::uint32_t Game::TargetHeight() const {
    return target_height_;
}

math::Vector3 Game::ScreenToWorld(const math::Point screen_point, const float depth) const {
    const auto x = static_cast<float>(screen_point.x);
    const auto y = static_cast<float>(screen_point.y);

    auto contains = [x, y](const Viewport &viewport) {
        return (viewport.x <= x && x < viewport.x + viewport.width) &&
               (viewport.y <= y && y < viewport.y + viewport.height);
    };

    for (const Viewport &viewport : ViewportManager().Viewports()) {
        if (!contains(viewport) || viewport.camera == nullptr) {
            continue;
        }
        const math::Vector3 point{x, y, std::clamp(depth, 0.0f, 1.0f)};
        const math::Matrix4x4 projection = viewport.camera->ProjectionMatrix();
        const math::Matrix4x4 view = viewport.camera->ViewMatrix();
        const math::Matrix4x4 world = math::Matrix4x4::Identity;
        return viewport.Unproject(point, projection, view, world);
    }

    return math::Vector3{std::numeric_limits<float>::quiet_NaN()};
}

math::Point Game::WorldToScreen(const math::Vector3 position, const Viewport *viewport) const {
    const Viewport &viewport_ref = (viewport != nullptr) ? *viewport : ViewportManager().TargetViewport();
    if (viewport_ref.camera == nullptr) {
        return math::Point{
            .x = -1,
            .y = -1,
        };
    }

    const math::Matrix4x4 projection = viewport_ref.camera->ProjectionMatrix();
    const math::Matrix4x4 view = viewport_ref.camera->ViewMatrix();
    const math::Matrix4x4 world = math::Matrix4x4::Identity;
    const math::Vector3 result = viewport_ref.Project(position, projection, view, world);
    return math::Point{
        .x = static_cast<std::int32_t>(result.x),
        .y = static_cast<std::int32_t>(result.y),
    };
}

const Timer &Game::Timer() const {
    return timer_;
}

const Window *Game::Window() const {
    return &window_;
}

Window *Game::Window() {
    return &window_;
}

const Input *Game::Input() const {
    return &input_;
}

Input *Game::Input() {
    return &input_;
}

bool Game::IsRunning() const {
    return is_running_;
}

void Game::Run() {
    if (is_running_) {
        return;
    }
    is_running_ = true;

    auto lag = Timer::Duration::zero();
    while (!should_exit_) {
        window_.ProcessQueueMessages();
        if (window_.IsDestroyed()) {
            break;
        }

        timer_.Tick();
        lag += timer_.CurrentTickTimePoint() - timer_.PreviousTickTimePoint();

        while (lag >= time_per_update_) {
            const float delta_time = Timer::SecondsFrom(time_per_update_);
            UpdateInternal(delta_time);
            lag -= time_per_update_;
        }

        DrawInternal();
    }

    is_running_ = false;
    should_exit_ = false;
}

void Game::Exit() {
    should_exit_ = true;
}

void Game::InitializeDevice() {
    constexpr std::array feature_level{D3D_FEATURE_LEVEL_11_1};
    const HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
                                             feature_level.data(), feature_level.size(), D3D11_SDK_VERSION, &device_,
                                             nullptr, &device_context_);
    detail::CheckResult(result, "Failed to create device");
}

void Game::InitializeSwapChain(const borov_engine::Window &window) {
    detail::D3DPtr<IDXGIDevice> dxgi_device;
    HRESULT result = device_.As(&dxgi_device);
    detail::CheckResult(result, "Failed to cast device to DXGI device");

    detail::D3DPtr<IDXGIAdapter> dxgi_adapter;
    result = dxgi_device->GetAdapter(&dxgi_adapter);
    detail::CheckResult(result, "Failed to retrieve DXGI adapter");

    detail::D3DPtr<IDXGIFactory> dxgi_factory;
    result = dxgi_adapter->GetParent(IID_IDXGIFactory, &dxgi_factory);
    detail::CheckResult(result, "Failed to retrieve DXGI factory");

    DXGI_SWAP_CHAIN_DESC swap_chain_desc{
        .BufferDesc =
            {
                .RefreshRate =
                    {
                        .Numerator = 60,
                        .Denominator = 1,
                    },
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                .Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
            },
        .SampleDesc =
            {
                .Count = 1,
                .Quality = 0,
            },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .OutputWindow = window.RawHandle(),
        .Windowed = true,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
    };
    result = dxgi_factory->CreateSwapChain(device_.Get(), &swap_chain_desc, &swap_chain_);
    detail::CheckResult(result, "Failed to create swap chain");
}

void Game::InitializeRenderTargetView() {
    HRESULT result;
    detail::D3DPtr<ID3D11Resource> resource;

    if (swap_chain_) {
        detail::D3DPtr<ID3D11Texture2D> back_buffer;
        result = swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, &back_buffer);
        detail::CheckResult(result, "Failed to create back buffer");

        D3D11_TEXTURE2D_DESC back_buffer_desc;
        back_buffer->GetDesc(&back_buffer_desc);
        target_width_ = back_buffer_desc.Width;
        target_height_ = back_buffer_desc.Height;

        resource = back_buffer;
    } else {
        throw std::runtime_error{"Failed to find any source to create render target from"};
    }

    result = device_->CreateRenderTargetView(resource.Get(), nullptr, &render_target_view_);
    detail::CheckResult(result, "Failed to create render target view");
}

void Game::InitializeDepthStencilView() {
    const D3D11_TEXTURE2D_DESC depth_buffer_desc{
        .Width = target_width_,
        .Height = target_height_,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc =
            DXGI_SAMPLE_DESC{
                .Count = 1,
                .Quality = 0,
            },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL,
    };
    HRESULT result = device_->CreateTexture2D(&depth_buffer_desc, nullptr, &depth_buffer_);
    detail::CheckResult(result, "Failed to create depth buffer");

    constexpr D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{
        .DepthEnable = true,
        .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
        .DepthFunc = D3D11_COMPARISON_LESS,
        .StencilEnable = false,
        .StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
        .StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
        .FrontFace =
            D3D11_DEPTH_STENCILOP_DESC{
                .StencilFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilPassOp = D3D11_STENCIL_OP_KEEP,
                .StencilFunc = D3D11_COMPARISON_ALWAYS,
            },
        .BackFace =
            D3D11_DEPTH_STENCILOP_DESC{
                .StencilFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilPassOp = D3D11_STENCIL_OP_KEEP,
                .StencilFunc = D3D11_COMPARISON_ALWAYS,
            },
    };
    result = device_->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state_);
    detail::CheckResult(result, "Failed to create depth stencil state");

    result = device_->CreateDepthStencilView(depth_buffer_.Get(), nullptr, &depth_stencil_view_);
    detail::CheckResult(result, "Failed to create depth stencil view");
}

void Game::InitializeShadowMapResources() {
    constexpr D3D11_TEXTURE2D_DESC shadow_map_desc{
        .Width = shadow_map_resolution,
        .Height = shadow_map_resolution,
        .MipLevels = 1,
        .ArraySize = shadow_map_cascade_count,
        .Format = DXGI_FORMAT_R32_TYPELESS,
        .SampleDesc =
            DXGI_SAMPLE_DESC{
                .Count = 1,
                .Quality = 0,
            },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
    };
    HRESULT result = device_->CreateTexture2D(&shadow_map_desc, nullptr, &shadow_map_);
    detail::CheckResult(result, "Failed to create shadow map depth");

    constexpr D3D11_DEPTH_STENCIL_VIEW_DESC shadow_map_depth_stencil_view_desc{
        .Format = DXGI_FORMAT_D32_FLOAT,
        .ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY,
        .Texture2DArray =
            D3D11_TEX2D_ARRAY_DSV{
                .ArraySize = shadow_map_cascade_count,
            },
    };
    result = device_->CreateDepthStencilView(shadow_map_.Get(), &shadow_map_depth_stencil_view_desc,
                                             &shadow_map_depth_view_);
    detail::CheckResult(result, "Failed to create shadow map depth stencil view");

    constexpr D3D11_SHADER_RESOURCE_VIEW_DESC shadow_map_shader_resource_view_desc{
        .Format = DXGI_FORMAT_R32_FLOAT,
        .ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY,
        .Texture2DArray =
            D3D11_TEX2D_ARRAY_SRV{
                .MipLevels = 1,
                .ArraySize = shadow_map_cascade_count,
            },
    };
    result = device_->CreateShaderResourceView(shadow_map_.Get(), &shadow_map_shader_resource_view_desc,
                                               &shadow_map_shader_resource_view_);
    detail::CheckResult(result, "Failed to create shadow map shader resource view");

    constexpr D3D11_SAMPLER_DESC shadow_map_sampler_desc{
        .Filter = D3D11_FILTER_ANISOTROPIC,
        .AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
        .AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
        .AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
        .ComparisonFunc = D3D11_COMPARISON_ALWAYS,
        .BorderColor = {0.0f, 0.0f, 0.0f, 0.0f},
        .MaxLOD = D3D11_FLOAT32_MAX,
    };
    result = device_->CreateSamplerState(&shadow_map_sampler_desc, &shadow_map_sampler_state_);
    detail::CheckResult(result, "Failed to create shadow map sampler state");

    const std::string shadow_map_cascade_count_definition = std::to_string(shadow_map_cascade_count);
    const std::array shadow_map_geometry_shader_defines{
        D3D_SHADER_MACRO{
            .Name = shadow_map_cascade_count_name.data(),
            .Definition = shadow_map_cascade_count_definition.c_str(),
        },
        D3D_SHADER_MACRO{},
    };
    shadow_map_geometry_shader_byte_code_ = detail::ShaderFromFile(
        "resources/shaders/game_cascade_shadow_map.hlsl", shadow_map_geometry_shader_defines.data(),
        D3D_COMPILE_STANDARD_FILE_INCLUDE, "GSMain", "gs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    result = device_->CreateGeometryShader(shadow_map_geometry_shader_byte_code_->GetBufferPointer(),
                                           shadow_map_geometry_shader_byte_code_->GetBufferSize(), nullptr,
                                           &shadow_map_geometry_shader_);
    detail::CheckResult(result, "Failed to create shadow map geometry shader from byte code");

    constexpr D3D11_BUFFER_DESC shadow_map_geometry_shader_buffer_desc{
        .ByteWidth = sizeof(ShadowMapConstantBuffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    result = device_->CreateBuffer(&shadow_map_geometry_shader_buffer_desc, nullptr, &shadow_map_constant_buffer_);
    detail::CheckResult(result, "Failed to create shadow map geometry shader constant buffer");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::UpdateShadowMapConstantBuffer(const ShadowMapConstantBuffer &data) {
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    const HRESULT result =
        device_context_->Map(shadow_map_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    detail::CheckResult(result, "Failed to map constant buffer data");

    std::memcpy(mapped_subresource.pData, &data, sizeof(data));
    device_context_->Unmap(shadow_map_constant_buffer_.Get(), 0);
}

void Game::UpdateInternal(const float delta_time) {
    Update(delta_time);

    if (camera_manager_ != nullptr) {
        camera_manager_->Update(delta_time);
    }
    viewport_manager_->Update(delta_time);
    debug_draw_->Update(delta_time);
}

void Game::Update(const float delta_time) {
    for (const auto &component : components_) {
        component->Update(delta_time);
    }
}

void Game::DrawInternal() {
    device_context_->ClearState();
    device_context_->ClearRenderTargetView(render_target_view_.Get(), clear_color_);
    device_context_->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    for (const auto &viewport : viewport_manager_->Viewports()) {
        Camera *camera = viewport.camera;

        device_context_->ClearState();

        constexpr std::array<ID3D11RenderTargetView *, 0> shadow_map_render_targets{};
        device_context_->OMSetRenderTargets(shadow_map_render_targets.size(), shadow_map_render_targets.data(),
                                            shadow_map_depth_view_.Get());

        device_context_->ClearDepthStencilView(shadow_map_depth_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                               1.0f, 0);

        const math::Viewport shadow_map_viewport{
            viewport.x, viewport.y, shadow_map_resolution, shadow_map_resolution, viewport.minDepth, viewport.maxDepth,
        };
        device_context_->RSSetViewports(1, shadow_map_viewport.Get11());

        constexpr std::array<ID3D11ClassInstance *, 0> gs_class_instances{};
        device_context_->GSSetShader(shadow_map_geometry_shader_.Get(), gs_class_instances.data(),
                                     gs_class_instances.size());

        // TODO: rework distances calculation
        // https://github.com/elizoorg/DirectXCourseProject/commit/db139bc41f27e34a9f757e014115f130c1c31f57#diff-3ddc8bf742cffeb52dd53848d6c7c5528f18e064486ae0e0a8741feb3db4a15cR11
        const float camera_near = camera != nullptr ? camera->NearPlane() : 0.0f;
        const float camera_far = camera != nullptr ? camera->FarPlane() : 0.0f;
        ShadowMapConstantBuffer shadow_map_constant_buffer{
            .distances =
                {
                    camera_near + (camera_far - camera_near) / 10.0f,
                    camera_near + (camera_far - camera_near) / 5.0f,
                    camera_near + (camera_far - camera_near) / 2.0f,
                    camera_far,
                },
        };
        for (std::uint8_t i = 0; i < shadow_map_cascade_count; ++i) {
            if (camera != nullptr && i != 0) {
                camera->NearPlane(shadow_map_constant_buffer.distances[i - 1]);
            }
            if (camera != nullptr && i != shadow_map_cascade_count - 1) {
                camera->FarPlane(shadow_map_constant_buffer.distances[i]);
            }

            shadow_map_constant_buffer.shadow_map_view_projections[i] = DirectionalLight().ViewMatrix(camera) *
                                                                        DirectionalLight().ProjectionMatrix(camera);
        }
        if (camera != nullptr) {
            camera->NearPlane(camera_near);
            camera->FarPlane(camera_far);
        }
        UpdateShadowMapConstantBuffer(shadow_map_constant_buffer);
        const std::array gs_constant_buffers{shadow_map_constant_buffer_.Get()};
        device_context_->GSSetConstantBuffers(0, gs_constant_buffers.size(), gs_constant_buffers.data());

        auto is_triangle = [](const Component &component) {
            return dynamic_cast<const TriangleComponent *>(&component) != nullptr;
        };
        auto to_triangle = [](Component &component) -> TriangleComponent & {
            return dynamic_cast<TriangleComponent &>(component);
        };
        // ReSharper disable once CppTooWideScopeInitStatement
        auto triangle_components = Components() | std::views::filter(is_triangle) | std::views::transform(to_triangle);
        for (TriangleComponent &component : triangle_components) {
            component.DrawInShadowMap(camera);
        }

        device_context_->ClearState();

        const std::array render_targets{render_target_view_.Get()};
        device_context_->OMSetRenderTargets(render_targets.size(), render_targets.data(), depth_stencil_view_.Get());
        device_context_->OMSetDepthStencilState(depth_stencil_state_.Get(), 1);

        const std::array shader_resources{shadow_map_shader_resource_view_.Get()};
        device_context_->PSSetShaderResources(0, shader_resources.size(), shader_resources.data());

        const std::array ps_constant_buffers{shadow_map_constant_buffer_.Get()};
        device_context_->PSSetConstantBuffers(0, ps_constant_buffers.size(), ps_constant_buffers.data());

        const std::array samplers{shadow_map_sampler_state_.Get()};
        device_context_->PSSetSamplers(0, samplers.size(), samplers.data());

        device_context_->RSSetViewports(1, viewport.Get11());

        Draw(camera);
        debug_draw_->Draw(camera);
    }
    Draw();

    if (camera_manager_ != nullptr) {
        camera_manager_->Draw(nullptr);
    }
    viewport_manager_->Draw(nullptr);

    constexpr std::array<ID3D11RenderTargetView *, 0> no_render_targets{};
    device_context_->OMSetRenderTargets(no_render_targets.size(), no_render_targets.data(), nullptr);
    device_context_->OMSetDepthStencilState(nullptr, 1);

    const HRESULT result = swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
    detail::CheckResult(result, "Failed to present into swapchain");
}

void Game::Draw() {}

void Game::Draw(const Camera *camera) {
    for (const auto &component : components_) {
        component->Draw(camera);
    }
}

void Game::OnTargetResize() {
    render_target_view_.Reset();
    if (swap_chain_ != nullptr) {
        const HRESULT result = swap_chain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        detail::CheckResult(result, "Failed to resize swap chain buffers");
    }
    InitializeRenderTargetView();
    InitializeDepthStencilView();

    if (camera_manager_ != nullptr) {
        camera_manager_->OnTargetResize();
    }
    viewport_manager_->OnTargetResize();

    for (const auto &component : components_) {
        component->OnTargetResize();
    }
}

void Game::OnWindowResize([[maybe_unused]] WindowResizeData data) {
    OnTargetResize();
}

}  // namespace borov_engine
