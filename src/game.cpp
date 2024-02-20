#include "borov_engine/game.hpp"

#include <winuser.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <exception>
#include <array>
#include <iostream>

namespace borov_engine {

namespace detail {

inline void CheckResult(const HRESULT result, const char *const message = nullptr) {
    if (FAILED(result)) {
        throw std::exception{message};
    }
}

}

Game::Game(Window &window) : window_{window} {
    InitializeDevice();
    InitializeSwapChain(window);
    InitializeRenderTargetView();
}

const Timer &Game::GetTimer() const {
    return timer_;
}

#ifdef UNICODE
#define sprintf_s swprintf_s
#else
#define sprintf_s sprintf_s
#endif

void Game::Run() {
    ID3DBlob *vertexBC = nullptr;
    ID3DBlob *errorVertexCode = nullptr;
    HRESULT res = D3DCompileFromFile(L"resources/shaders/shader.hlsl",
                                     nullptr /*macros*/,
                                     nullptr /*include*/,
                                     "VSMain",
                                     "vs_5_0",
                                     D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                     0,
                                     &vertexBC,
                                     &errorVertexCode);

    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            char *compileErrors = (char *) (errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
            // If there was  nothing in the error message then it simply could not find the shader file itself.
        else {
            MessageBox(window_.GetRawHandle(), TEXT("MyVeryFirstShader.hlsl"), TEXT("Missing Shader File"), MB_OK);
        }

        return;
    }

    D3D_SHADER_MACRO Shader_Macros[] = {"TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

    ID3DBlob *pixelBC;
    ID3DBlob *errorPixelCode;
    res = D3DCompileFromFile(L"resources/shaders/shader.hlsl",
                             Shader_Macros /*macros*/,
                             nullptr /*include*/,
                             "PSMain",
                             "ps_5_0",
                             D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                             0,
                             &pixelBC,
                             &errorPixelCode);

    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    device_->CreateVertexShader(
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        nullptr, &vertexShader);

    device_->CreatePixelShader(
        pixelBC->GetBufferPointer(),
        pixelBC->GetBufferSize(),
        nullptr, &pixelShader);

    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC{
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0},
        D3D11_INPUT_ELEMENT_DESC{
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0}
    };

    ID3D11InputLayout *layout;
    device_->CreateInputLayout(
        inputElements,
        2,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        &layout);

    DirectX::XMFLOAT4 points[8] = {
        DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
    };

    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = points;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    ID3D11Buffer *vb;
    device_->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

    int indices[] = {0, 1, 2, 1, 0, 3};
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(int) * std::size(indices);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    ID3D11Buffer *ib;
    device_->CreateBuffer(&indexBufDesc, &indexData, &ib);

    UINT strides[] = {32};
    UINT offsets[] = {0};

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    ID3D11RasterizerState *rastState;
    res = device_->CreateRasterizerState(&rastDesc, &rastState);

    device_context_->RSSetState(rastState);

    MSG msg = {};
    bool isExitRequested = false;
    while (!isExitRequested) {
        // Handle the windows messages.
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If windows signals to end the application then exit out.
        if (msg.message == WM_QUIT) {
            isExitRequested = true;
        }

        device_context_->ClearState();

        device_context_->RSSetState(rastState);

        D3D11_VIEWPORT viewport = {};
        viewport.Width = 800;
        viewport.Height = 800;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1.0f;

        device_context_->RSSetViewports(1, &viewport);

        device_context_->IASetInputLayout(layout);
        device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        device_context_->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
        device_context_->IASetVertexBuffers(0, 1, &vb, strides, offsets);
        device_context_->VSSetShader(vertexShader, nullptr, 0);
        device_context_->PSSetShader(pixelShader, nullptr, 0);

        timer_.Tick();

        TCHAR text[256];
        sprintf_s(text, TEXT("FPS: %f"), timer_.FramesPerSecond());
        window_.SetTitle(text);

        device_context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), nullptr);

        float start_time = timer_.StartTime();
        float red = start_time - std::floor(start_time);
        float color[] = {red, 0.1f, 0.1f, 1.0f};
        device_context_->ClearRenderTargetView(render_target_view_.Get(), color);

        device_context_->DrawIndexed(6, 0, 0);

        device_context_->OMSetRenderTargets(0, nullptr, nullptr);

        swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
    }
}

#undef sprintf_s

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

    ComPtr<IDXGIDevice> dxgi_device;
    result = device_.As(&dxgi_device);
    detail::CheckResult(result, "Failed to cast device to DXGI device");

    ComPtr<IDXGIAdapter> dxgi_adapter;
    result = dxgi_device->GetAdapter(&dxgi_adapter);
    detail::CheckResult(result, "Failed to retrieve DXGI adapter");

    ComPtr<IDXGIFactory> dxgi_factory;
    result = dxgi_adapter->GetParent(IID_IDXGIFactory, &dxgi_factory);
    detail::CheckResult(result, "Failed to retrieve DXGI factory");

    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {
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

    ComPtr<ID3D11Texture2D> back_buffer;
    result = swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, &back_buffer);
    detail::CheckResult(result, "Failed to create back buffer");

    result = device_->CreateRenderTargetView(back_buffer.Get(), nullptr, &render_target_view_);
    detail::CheckResult(result, "Failed to create render target view");
}

}
