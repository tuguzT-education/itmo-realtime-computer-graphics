#include "borov_engine/game.hpp"

#include <array>
#include <format>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/camera_manager.hpp"
#include "borov_engine/viewport_manager.hpp"

namespace borov_engine {

constexpr Timer::Duration default_time_per_update = std::chrono::microseconds{6500};

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
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

    ViewportManager<borov_engine::ViewportManager>();

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

const ViewportManager &Game::ViewportManager() const {
    return *viewport_manager_;
}

ViewportManager &Game::ViewportManager() {
    return *viewport_manager_;
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
            float delta_time = Timer::SecondsFrom(time_per_update_);
            Update(delta_time);
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
    std::array feature_level{D3D_FEATURE_LEVEL_11_1};
    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        feature_level.data(),
        feature_level.size(),
        D3D11_SDK_VERSION,
        &device_,
        nullptr,
        &device_context_);
    detail::CheckResult(result, "Failed to create device");
}

void Game::InitializeSwapChain(const borov_engine::Window &window) {
    HRESULT result;

    detail::D3DPtr<IDXGIDevice> dxgi_device;
    result = device_.As(&dxgi_device);
    detail::CheckResult(result, "Failed to cast device to DXGI device");

    detail::D3DPtr<IDXGIAdapter> dxgi_adapter;
    result = dxgi_device->GetAdapter(&dxgi_adapter);
    detail::CheckResult(result, "Failed to retrieve DXGI adapter");

    detail::D3DPtr<IDXGIFactory> dxgi_factory;
    result = dxgi_adapter->GetParent(IID_IDXGIFactory, &dxgi_factory);
    detail::CheckResult(result, "Failed to retrieve DXGI factory");

    DXGI_SWAP_CHAIN_DESC swap_chain_desc{
        .BufferDesc = {
            .RefreshRate = {
                .Numerator = 60,
                .Denominator = 1,
            },
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
            .Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
        },
        .SampleDesc = {
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
    result = dxgi_factory->CreateSwapChain(
        device_.Get(),
        &swap_chain_desc,
        &swap_chain_);
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

void Game::Update(float delta_time) {
    if (camera_manager_ != nullptr) {
        camera_manager_->Update(delta_time);
    }
    viewport_manager_->Update(delta_time);

    for (const auto &component : components_) {
        component->Update(delta_time);
    }
}

void Game::Draw() {
    if (camera_manager_ != nullptr) {
        camera_manager_->Draw(nullptr);
    }
    viewport_manager_->Draw(nullptr);

    for (const auto &viewport : viewport_manager_->Viewports()) {
        device_context_->RSSetViewports(1, viewport.Get11());

        Camera *camera = viewport.camera;
        if (camera != nullptr) {
            camera->Width(viewport.width);
            camera->Height(viewport.height);
        }

        for (const auto &component : components_) {
            component->Draw(camera);
        }
    }
}

void Game::OnTargetResize() {
    render_target_view_.Reset();
    if (swap_chain_ != nullptr) {
        HRESULT result = swap_chain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        detail::CheckResult(result, "Failed to resize swap chain buffers");
    }
    InitializeRenderTargetView();

    if (camera_manager_ != nullptr) {
        camera_manager_->OnTargetResize();
    }
    viewport_manager_->OnTargetResize();

    for (const auto &component : components_) {
        component->OnTargetResize();
    }
}

void Game::DrawInternal() {
    device_context_->ClearState();

    std::array render_targets{render_target_view_.Get()};
    device_context_->OMSetRenderTargets(render_targets.size(), render_targets.data(), nullptr);

    device_context_->ClearRenderTargetView(render_target_view_.Get(), clear_color_);

    Draw();

    std::array<ID3D11RenderTargetView *, 0> no_render_targets;
    device_context_->OMSetRenderTargets(no_render_targets.size(), no_render_targets.data(), nullptr);

    swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::OnWindowResize([[maybe_unused]] WindowResizeData data) {
    OnTargetResize();
}

}
