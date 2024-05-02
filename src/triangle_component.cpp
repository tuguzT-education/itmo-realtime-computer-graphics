#include "borov_engine/triangle_component.hpp"

#include <WICTextureLoader.h>
#include <d3dcompiler.h>

#include <array>
#include <format>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"
#include "borov_engine/detail/texture.hpp"

namespace borov_engine {

TriangleComponent::TriangleComponent(borov_engine::Game &game, const std::span<const Vertex> vertices,
                                     const std::span<const Index> indices, const std::string_view texture_path,
                                     const bool wireframe, const borov_engine::Transform &transform,
                                     const SceneComponent *parent)
    : SceneComponent{game, transform, parent}, wireframe_{wireframe} {
    InitializeVertexShader();
    InitializeIndexShader();
    InitializeInputLayout();
    InitializeRasterizerState();
    InitializeSamplerState();
    InitializeConstantBuffer(ConstantBuffer{});

    Load(vertices, indices);
    LoadTexture(texture_path);
}

void TriangleComponent::Load(const std::span<const Vertex> vertices, const std::span<const Index> indices) {
    InitializeVertexBuffer(vertices);
    InitializeIndexBuffer(indices);
}

void TriangleComponent::LoadTexture(const std::string_view texture_path) {
    if (texture_path.empty()) {
        return;
    }

    texture_ = detail::TextureFromFile(Device(), texture_path);
}

bool TriangleComponent::Wireframe() const {
    return wireframe_;
}

void TriangleComponent::Wireframe(const bool wireframe) {
    wireframe_ = wireframe;
    InitializeRasterizerState();
}

void TriangleComponent::Draw(const Camera *camera) {
    ID3D11DeviceContext &device_context = DeviceContext();

    const std::array vertex_buffers = {vertex_buffer_.Get()};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> strides{sizeof(Vertex)};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> offsets{0};

    device_context.RSSetState(rasterizer_state_.Get());
    device_context.IASetInputLayout(input_layout_.Get());
    device_context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device_context.IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
    device_context.IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());
    device_context.VSSetShader(vertex_shader_.Get(), nullptr, 0);
    device_context.PSSetShader(index_shader_.Get(), nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE subresource{};
    const ConstantBuffer constant_buffer{
        .world = WorldTransform().ToMatrix(),
        .view = (camera != nullptr) ? camera->View() : math::Matrix4x4::Identity,
        .projection = (camera != nullptr) ? camera->Projection() : math::Matrix4x4::Identity,
        .has_texture = texture_ != nullptr,
    };
    const HRESULT result = device_context.Map(constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
    detail::CheckResult(result, "Failed to map constant buffer data");
    std::memcpy(subresource.pData, &constant_buffer, sizeof(constant_buffer));
    device_context.Unmap(constant_buffer_.Get(), 0);

    const std::array constant_buffers{constant_buffer_.Get()};
    device_context.VSSetConstantBuffers(0, constant_buffers.size(), constant_buffers.data());
    device_context.PSSetConstantBuffers(0, constant_buffers.size(), constant_buffers.data());

    const std::array shader_resources{texture_.Get()};
    device_context.PSSetShaderResources(0, shader_resources.size(), shader_resources.data());

    const std::array sampler_states{sampler_state_.Get()};
    device_context.PSSetSamplers(0, sampler_states.size(), sampler_states.data());

    D3D11_BUFFER_DESC index_buffer_desc;
    index_buffer_->GetDesc(&index_buffer_desc);
    const UINT index_count = index_buffer_desc.ByteWidth / sizeof(Index);
    device_context.DrawIndexed(index_count, 0, 0);
}

void TriangleComponent::InitializeVertexShader() {
    vertex_byte_code_ = detail::ShaderFromFile("resources/shaders/triangle_component.hlsl", nullptr, nullptr, "VSMain",
                                               "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(vertex_byte_code_->GetBufferPointer(),
                                                       vertex_byte_code_->GetBufferSize(), nullptr, &vertex_shader_);
    detail::CheckResult(result, "Failed to create vertex shader from byte code");
}

void TriangleComponent::InitializeIndexShader() {
    index_byte_code_ = detail::ShaderFromFile("resources/shaders/triangle_component.hlsl", nullptr, nullptr, "PSMain",
                                              "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreatePixelShader(index_byte_code_->GetBufferPointer(),
                                                      index_byte_code_->GetBufferSize(), nullptr, &index_shader_);
    detail::CheckResult(result, "Failed to create index shader from byte code");
}

void TriangleComponent::InitializeInputLayout() {
    const std::array input_elements = std::to_array(Vertex::InputElements);
    const HRESULT result = Device().CreateInputLayout(input_elements.data(), input_elements.size(),
                                                      vertex_byte_code_->GetBufferPointer(),
                                                      vertex_byte_code_->GetBufferSize(), &input_layout_);
    detail::CheckResult(result, "Failed to create input layout");
}

void TriangleComponent::InitializeRasterizerState() {
    const D3D11_RASTERIZER_DESC rasterizer_desc{
        .FillMode = wireframe_ ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_FRONT,
    };

    const HRESULT result = Device().CreateRasterizerState(&rasterizer_desc, &rasterizer_state_);
    detail::CheckResult(result, "Failed to create rasterizer state");
}

void TriangleComponent::InitializeSamplerState() {
    constexpr D3D11_SAMPLER_DESC sampler_desc{
        .Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        .AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
        .AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
        .AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
        .ComparisonFunc = D3D11_COMPARISON_ALWAYS,
        .BorderColor = {1.0f, 0.0f, 0.0f, 1.0f},
        .MaxLOD = D3D11_FLOAT32_MAX,
    };

    const HRESULT result = Device().CreateSamplerState(&sampler_desc, &sampler_state_);
    detail::CheckResult(result, "Failed to create sampler state");
}

void TriangleComponent::InitializeVertexBuffer(std::span<const Vertex> vertices) {
    const D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = static_cast<std::uint32_t>(vertices.size_bytes()),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    const D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = vertices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, &initial_data, &vertex_buffer_);
    detail::CheckResult(result, "Failed to create vertex buffer");
}

void TriangleComponent::InitializeIndexBuffer(std::span<const Index> indices) {
    const D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = static_cast<UINT>(indices.size_bytes()),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_INDEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    const D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = indices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, &initial_data, &index_buffer_);
    detail::CheckResult(result, "Failed to create index buffer");
}

void TriangleComponent::InitializeConstantBuffer(ConstantBuffer constant_buffer) {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(constant_buffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };
    const D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = &constant_buffer,
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, &initial_data, &constant_buffer_);
    detail::CheckResult(result, "Failed to create constant buffer");
}

}  // namespace borov_engine
