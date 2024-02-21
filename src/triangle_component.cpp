#include "borov_engine/triangle_component.hpp"

#include <d3dcompiler.h>

#include <format>
#include <array>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/string_api_set.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

namespace detail {

D3DPtr<ID3DBlob> CompileFromFile(const char *path,
                                 const D3D_SHADER_MACRO *defines,
                                 ID3DInclude *include,
                                 const char *entrypoint,
                                 const char *target,
                                 UINT flags_1,
                                 UINT flags_2) {
    D3DPtr<ID3DBlob> shader;

    D3DPtr<ID3DBlob> error_messages;
    std::wstring w_path = MultiByteToWideChar(CP_UTF8, 0, path);
    HRESULT result = D3DCompileFromFile(w_path.c_str(),
                                        defines,
                                        include,
                                        entrypoint,
                                        target,
                                        flags_1,
                                        flags_2,
                                        &shader,
                                        &error_messages);
    detail::CheckResult(result, [&] {
        const char *message = error_messages
                              ? static_cast<const char *>(error_messages->GetBufferPointer())
                              : "file is missing";
        return std::format("Failed to compile vertex shader file '{}': {}", path, message);
    });

    return shader;
}

}

TriangleComponent::TriangleComponent(Game &game,
                                     std::span<Vertex> vertices,
                                     std::span<Index> indices) : Component{game} {
    InitializeVertexShader();
    InitializeIndexShader();
    InitializeInputLayout();
    InitializeRasterizerState();
    InitializeVertexBuffer(vertices);
    InitializeIndexBuffer(indices);
}

void TriangleComponent::Update(float delta_time) {}

void TriangleComponent::Draw() {
    ID3D11DeviceContext *device_context = GetDeviceContext();

    std::array vertex_buffers = {vertex_buffer_.Get()};
    std::array<UINT, vertex_buffers.size()> strides{sizeof(Vertex)};
    std::array<UINT, vertex_buffers.size()> offsets{0u};

    device_context->RSSetState(rasterizer_state_.Get());
    device_context->IASetInputLayout(input_layout_.Get());
    device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device_context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
    device_context->IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());
    device_context->VSSetShader(vertex_shader_.Get(), nullptr, 0);
    device_context->PSSetShader(index_shader_.Get(), nullptr, 0);

    device_context->DrawIndexed(6, 0, 0);
}

void TriangleComponent::InitializeVertexShader() {
    vertex_byte_code_ = detail::CompileFromFile("resources/shaders/shader.hlsl",
                                                nullptr,
                                                nullptr,
                                                "VSMain",
                                                "vs_5_0",
                                                D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                                0);

    HRESULT result = GetDevice()->CreateVertexShader(vertex_byte_code_->GetBufferPointer(),
                                                     vertex_byte_code_->GetBufferSize(),
                                                     nullptr, &vertex_shader_);
    detail::CheckResult(result, "Failed to create vertex shader from byte code");
}

void TriangleComponent::InitializeIndexShader() {
    std::array shader_macros{
        D3D_SHADER_MACRO{.Name = "TEST", .Definition = "1"},
        D3D_SHADER_MACRO{.Name = "TCOLOR", .Definition = "float4(0.0f, 1.0f, 0.0f, 1.0f)"},
        D3D_SHADER_MACRO{.Name = nullptr, .Definition = nullptr},
    };
    index_byte_code_ = detail::CompileFromFile("resources/shaders/shader.hlsl",
                                               shader_macros.data(),
                                               nullptr,
                                               "PSMain",
                                               "ps_5_0",
                                               D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                               0);

    HRESULT result = GetDevice()->CreatePixelShader(index_byte_code_->GetBufferPointer(),
                                                    index_byte_code_->GetBufferSize(),
                                                    nullptr, &index_shader_);
    detail::CheckResult(result, "Failed to create index shader from byte code");
}

void TriangleComponent::InitializeInputLayout() {
    std::array input_elements{
        D3D11_INPUT_ELEMENT_DESC{
            .SemanticName = "POSITION",
            .SemanticIndex = 0,
            .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            .InputSlot = 0,
            .AlignedByteOffset = 0,
            .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0,
        },
        D3D11_INPUT_ELEMENT_DESC{
            .SemanticName = "COLOR",
            .SemanticIndex = 0,
            .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            .InputSlot = 0,
            .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
            .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0,
        },
    };

    HRESULT result = GetDevice()->CreateInputLayout(
        input_elements.data(),
        input_elements.size(),
        vertex_byte_code_->GetBufferPointer(),
        vertex_byte_code_->GetBufferSize(),
        &input_layout_);
    detail::CheckResult(result, "Failed to create input layout");
}

void TriangleComponent::InitializeRasterizerState() {
    D3D11_RASTERIZER_DESC rasterizer_desc{
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_NONE,
    };

    HRESULT result = GetDevice()->CreateRasterizerState(&rasterizer_desc, &rasterizer_state_);
    detail::CheckResult(result, "Failed to create rasterizer state");
}

void TriangleComponent::InitializeVertexBuffer(std::span<Vertex> vertices) {
    D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = static_cast<UINT>(vertices.size_bytes()),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = vertices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    HRESULT result = GetDevice()->CreateBuffer(&buffer_desc, &initial_data, &vertex_buffer_);
    detail::CheckResult(result, "Failed to create vertex buffer");
}

void TriangleComponent::InitializeIndexBuffer(std::span<Index> indices) {
    D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = static_cast<UINT>(indices.size_bytes()),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_INDEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = indices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    HRESULT result = GetDevice()->CreateBuffer(&buffer_desc, &initial_data, &index_buffer_);
    detail::CheckResult(result, "Failed to create index buffer");
}

}
