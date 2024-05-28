#include "player_child.hpp"

#include <d3dcompiler.h>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"

PlayerChild::PlayerChild(borov_engine::Game& game, const Initializer& initializer)
    : TriangleComponent(game, initializer) {
    ReInitializeVertexShader();
    ReInitializeVertexShaderConstantBuffer();
}

void PlayerChild::ReInitializeVertexShader() {
    vertex_shader_byte_code_ = borov_engine::detail::ShaderFromFile("resources/shaders/katamari_player.hlsl", nullptr,
                                                             D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0",
                                                             D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(vertex_shader_byte_code_->GetBufferPointer(),
                                                       vertex_shader_byte_code_->GetBufferSize(), nullptr, &vertex_shader_);
    borov_engine::detail::CheckResult(result, "Failed to create vertex shader from byte code");
}

void PlayerChild::ReInitializeVertexShaderConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(VertexShaderConstantBuffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &vertex_shader_constant_buffer_);
    borov_engine::detail::CheckResult(result, "Failed to create vertex shader constant buffer");
}

void PlayerChild::UpdateVertexShaderConstantBuffer(const TriangleComponent::VertexShaderConstantBuffer& data) {
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    const HRESULT result =
        DeviceContext().Map(vertex_shader_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    borov_engine::detail::CheckResult(result, "Failed to map constant buffer data");

    VertexShaderConstantBuffer new_data{data};
    new_data.time = Game().Timer().StartTime();

    std::memcpy(mapped_subresource.pData, &new_data, sizeof(new_data));
    DeviceContext().Unmap(vertex_shader_constant_buffer_.Get(), 0);
}
