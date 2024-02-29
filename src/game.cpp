#include "borov_engine/game.hpp"

#include <array>
#include <format>

#include <SimpleMath.h>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/triangle_component.hpp"

namespace borov_engine {

constexpr Timer::Duration default_time_per_update = std::chrono::microseconds{6500};

Game::Game(Window &window, Input &input)
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
}

Game::~Game() = default;

const Timer::Duration &Game::TimePerUpdate() const {
    return time_per_update_;
}

void Game::TimePerUpdate(Timer::Duration time_per_update) {
    time_per_update_ = time_per_update;
}

const math::Color &Game::ClearColor() const {
    return clear_color_;
}

math::Color &Game::ClearColor() {
    return clear_color_;
}

bool Game::IsRunning() const {
    return is_running_;
}

std::span<const std::unique_ptr<Component>> Game::Components() const {
    return components_;
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

        Draw();
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

void Game::InitializeSwapChain(const Window &window) {
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
    for (const auto &component : components_) {
        component->Update(delta_time);
    }
}

void Game::Draw() {
    device_context_->ClearState();

    D3D11_VIEWPORT viewport{
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<FLOAT>(target_width_),
        .Height = static_cast<FLOAT>(target_height_),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };
    std::array viewports{viewport};
    device_context_->RSSetViewports(viewports.size(), viewports.data());

    std::array render_targets{render_target_view_.Get()};
    device_context_->OMSetRenderTargets(render_targets.size(), render_targets.data(), nullptr);

    device_context_->ClearRenderTargetView(render_target_view_.Get(), clear_color_);

    for (const auto &component : components_) {
        component->Draw();
    }

    std::array<ID3D11RenderTargetView *, 0> no_render_targets;
    device_context_->OMSetRenderTargets(no_render_targets.size(), no_render_targets.data(), nullptr);

    swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

}
