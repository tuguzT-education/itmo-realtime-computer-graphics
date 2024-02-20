#include <borov_engine/window.hpp>
#include <borov_engine/timer.hpp>
#include <borov_engine/game.hpp>
#include <borov_engine/delegate/delegate.hpp>
#include <borov_engine/delegate/multicast_delegate.hpp>

#include <winuser.h>
#include <wrl.h>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef UNICODE
#define sprintf_s swprintf_s
#else
#define sprintf_s sprintf_s
#endif

int main() {
    constexpr LONG width = 800, height = 800;
    borov_engine::Window window{TEXT("Application"), width, height, GetModuleHandle(nullptr)};
    borov_engine::Game game{window};
    borov_engine::Timer timer{};

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
            MessageBox(window.GetRawHandle(), TEXT("MyVeryFirstShader.hlsl"), TEXT("Missing Shader File"), MB_OK);
        }

        return 0;
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
    game.device_->CreateVertexShader(
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        nullptr, &vertexShader);

    game.device_->CreatePixelShader(
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
    game.device_->CreateInputLayout(
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
    game.device_->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

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
    game.device_->CreateBuffer(&indexBufDesc, &indexData, &ib);

    UINT strides[] = {32};
    UINT offsets[] = {0};

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    ID3D11RasterizerState *rastState;
    res = game.device_->CreateRasterizerState(&rastDesc, &rastState);

    game.device_context_->RSSetState(rastState);

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

        game.device_context_->ClearState();

        game.device_context_->RSSetState(rastState);

        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1.0f;

        game.device_context_->RSSetViewports(1, &viewport);

        game.device_context_->IASetInputLayout(layout);
        game.device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        game.device_context_->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
        game.device_context_->IASetVertexBuffers(0, 1, &vb, strides, offsets);
        game.device_context_->VSSetShader(vertexShader, nullptr, 0);
        game.device_context_->PSSetShader(pixelShader, nullptr, 0);

        timer.Tick();

        TCHAR text[256];
        sprintf_s(text, TEXT("FPS: %f"), timer.FramesPerSecond());
        window.SetTitle(text);

        game.device_context_->OMSetRenderTargets(1, game.render_target_view_.GetAddressOf(), nullptr);

        float integral;
        float red = std::modf(timer.StartTime(), &integral);
        float color[] = {red, 0.1f, 0.1f, 1.0f};
        game.device_context_->ClearRenderTargetView(game.render_target_view_.Get(), color);

        game.device_context_->DrawIndexed(6, 0, 0);

        game.device_context_->OMSetRenderTargets(0, nullptr, nullptr);

        game.swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
    }

    std::cout << "Hello World!\n";

    borov_engine::delegate::Delegate<int> delegate;
    delegate.BindLambda([] { return 42; });
    auto result = delegate.ExecuteIfBound();
    std::cout << result.value() << std::endl;

    borov_engine::delegate::MulticastDelegate multicast_delegate;
    multicast_delegate.AddLambda([&result] { std::cout << result.value() << std::endl; });
    multicast_delegate.Broadcast();

    return 0;
}
