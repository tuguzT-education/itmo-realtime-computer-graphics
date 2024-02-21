#include "borov_engine/game.hpp"

#include <array>
#include <iostream>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/triangle_component.hpp"

namespace borov_engine {

Game::Game(Window &window) : window_{window} {
    auto [width, height] = window.GetClientDimensions();
    initial_width_ = width;
    initial_height_ = height;

    InitializeDevice();
    InitializeSwapChain(window);
    InitializeRenderTargetView();

    std::array vertices{
        TriangleComponent::Vertex{
            .position = {0.5f, 0.5f, 0.5f, 1.0f},
            .color = {1.0f, 0.0f, 0.0f, 1.0f},
        },
        TriangleComponent::Vertex{
            .position = {-0.5f, -0.5f, 0.5f, 1.0f},
            .color = {0.0f, 0.0f, 1.0f, 1.0f},
        },
        TriangleComponent::Vertex{
            .position = {0.5f, -0.5f, 0.5f, 1.0f},
            .color = {0.0f, 1.0f, 0.0f, 1.0f},
        },
        TriangleComponent::Vertex{
            .position = {-0.5f, 0.5f, 0.5f, 1.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f},
        },
    };
    std::array indices{0, 1, 2, 1, 0, 3};
    auto triangle_component = std::make_unique<TriangleComponent>(*this, vertices, indices);
    components_.push_back(std::move(triangle_component));
}

#ifdef UNICODE
#define sprintf_s swprintf_s
#else
#define sprintf_s sprintf_s
#endif

void Game::Run() {
    MSG msg{};
    while (true) {
        // Handle the windows messages.
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // If Windows signals to end the application then exit out.
        if (msg.message == WM_QUIT) {
            break;
        }

        timer_.Tick();

        TCHAR text[256];
        sprintf_s(text, TEXT("FPS: %f"), timer_.FramesPerSecond());
        window_.SetTitle(text);

        Draw();
    }
}

#undef sprintf_s

void Game::Exit() {
    window_.Destroy();
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
        .OutputWindow = window.GetRawHandle(),
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

    detail::D3DPtr<ID3D11Texture2D> back_buffer;
    result = swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, &back_buffer);
    detail::CheckResult(result, "Failed to create back buffer");

    result = device_->CreateRenderTargetView(back_buffer.Get(), nullptr, &render_target_view_);
    detail::CheckResult(result, "Failed to create render target view");
}

void Game::Draw() {
    device_context_->ClearState();

    D3D11_VIEWPORT viewport{
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width = static_cast<FLOAT>(initial_width_),
        .Height = static_cast<FLOAT>(initial_height_),
        .MinDepth = 0,
        .MaxDepth = 1.0f,
    };
    std::array viewports{viewport};
    device_context_->RSSetViewports(viewports.size(), viewports.data());

    device_context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), nullptr);

    float start_time = timer_.StartTime();
    float red = start_time - std::floor(start_time);
    DirectX::XMFLOAT4 color{red, 0.1f, 0.1f, 1.0f};
    device_context_->ClearRenderTargetView(render_target_view_.Get(), &color.x);

    for (const auto &component : components_) {
        component->Draw();
    }

    device_context_->OMSetRenderTargets(0, nullptr, nullptr);

    swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

}
