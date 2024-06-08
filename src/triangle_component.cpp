#include "borov_engine/triangle_component.hpp"

#include <d3dcompiler.h>

#undef min
#undef max

#include <array>
#include <range/v3/view/enumerate.hpp>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"
#include "borov_engine/detail/texture.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

TriangleComponent::TriangleComponent(class Game &game, const Initializer &initializer)
    : SceneComponent(game, initializer),
      tile_count_{math::Vector2::One},
      wireframe_{initializer.wireframe},
      prev_wireframe_{initializer.wireframe},
      is_casting_shadow_{initializer.is_casting_shadow} {
    InitializeVertexShader();
    InitializeVertexShaderConstantBuffer();

    InitializePixelShader();
    InitializePixelShaderConstantBuffer();

    InitializeShadowMapVertexShader();
    InitializeShadowMapRasterizerState();

    InitializeInputLayout();
    InitializeRasterizerState();
    InitializeTextureSamplerState();

    Load(initializer.vertices, initializer.indices);
    LoadTexture(initializer.texture_path, initializer.tile_count);
    material_ = initializer.material;
}

void TriangleComponent::Load(const std::span<const Vertex> vertices, const std::span<const Index> indices) {
    InitializeVertexBuffer(vertices);
    InitializeIndexBuffer(indices);
}

void TriangleComponent::LoadTexture(const std::filesystem::path &texture_path, const math::Vector2 tile_count) {
    if (!texture_path.has_filename()) {
        return;
    }

    texture_ = detail::TextureFromFile(Device(), DeviceContext(), texture_path);
    tile_count_ = tile_count;
}

bool TriangleComponent::Wireframe() const {
    return wireframe_;
}

bool &TriangleComponent::Wireframe() {
    return wireframe_;
}

bool TriangleComponent::IsCastingShadow() const {
    return is_casting_shadow_;
}

bool &TriangleComponent::IsCastingShadow() {
    return is_casting_shadow_;
}

const Material &TriangleComponent::Material() const {
    return material_;
}

Material &TriangleComponent::Material() {
    return material_;
}

void TriangleComponent::DrawInShadowMap(const Camera *camera) {
    if (!is_casting_shadow_ || vertex_buffer_ == nullptr || index_buffer_ == nullptr) {
        return;
    }

    ID3D11DeviceContext &device_context = DeviceContext();

    device_context.RSSetState(shadow_map_rasterizer_state_.Get());
    device_context.IASetInputLayout(input_layout_.Get());
    device_context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr std::array<ID3D11ClassInstance *, 0> vs_class_instances{};
    device_context.VSSetShader(shadow_map_vertex_shader_.Get(), vs_class_instances.data(), vs_class_instances.size());

    const VertexShaderConstantBuffer vs_constant_buffer{
        .world = WorldTransform().ToMatrix(),
        .view = (camera != nullptr) ? camera->ViewMatrix() : math::Matrix4x4::Identity,
        .projection = (camera != nullptr) ? camera->ProjectionMatrix() : math::Matrix4x4::Identity,
        .tile_count = tile_count_,
    };
    UpdateVertexShaderConstantBuffer(vs_constant_buffer);
    const std::array vs_constant_buffers{vertex_shader_constant_buffer_.Get()};
    device_context.VSSetConstantBuffers(0, vs_constant_buffers.size(), vs_constant_buffers.data());

    constexpr std::array<ID3D11ClassInstance *, 0> ps_class_instances{};
    device_context.PSSetShader(nullptr, ps_class_instances.data(), ps_class_instances.size());

    const std::array vertex_buffers = {vertex_buffer_.Get()};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> strides{sizeof(Vertex)};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> offsets{};
    device_context.IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());
    device_context.IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

    D3D11_BUFFER_DESC index_buffer_desc;
    index_buffer_->GetDesc(&index_buffer_desc);
    const std::uint32_t index_count = index_buffer_desc.ByteWidth / sizeof(Index);
    device_context.DrawIndexed(index_count, 0, 0);
}

void TriangleComponent::Draw(const Camera *camera) {
    if (vertex_buffer_ == nullptr || index_buffer_ == nullptr) {
        return;
    }

    if (wireframe_ != prev_wireframe_) {
        InitializeRasterizerState();
        prev_wireframe_ = wireframe_;
    }

    ID3D11DeviceContext &device_context = DeviceContext();

    device_context.RSSetState(rasterizer_state_.Get());
    device_context.IASetInputLayout(input_layout_.Get());
    device_context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr std::array<ID3D11ClassInstance *, 0> vs_class_instances{};
    device_context.VSSetShader(vertex_shader_.Get(), vs_class_instances.data(), vs_class_instances.size());

    const VertexShaderConstantBuffer vs_constant_buffer{
        .world = WorldTransform().ToMatrix(),
        .view = (camera != nullptr) ? camera->ViewMatrix() : math::Matrix4x4::Identity,
        .projection = (camera != nullptr) ? camera->ProjectionMatrix() : math::Matrix4x4::Identity,
        .tile_count = tile_count_,
    };
    UpdateVertexShaderConstantBuffer(vs_constant_buffer);
    const std::array vs_constant_buffers{vertex_shader_constant_buffer_.Get()};
    device_context.VSSetConstantBuffers(0, vs_constant_buffers.size(), vs_constant_buffers.data());

    constexpr std::array<ID3D11ClassInstance *, 0> ps_class_instances{};
    device_context.PSSetShader(pixel_shader_.Get(), ps_class_instances.data(), ps_class_instances.size());

    const PixelShaderConstantBuffer ps_constant_buffer{
        .has_texture = texture_ != nullptr,
        .view_position = (camera != nullptr) ? camera->WorldTransform().position : math::Vector3::Backward,
        .material = material_,
        .directional_light = Game().DirectionalLight().DirectionalLight(),
        .point_light = Game().PointLight().PointLight(),
        .spot_light = Game().SpotLight().SpotLight(),
    };
    UpdatePixelShaderConstantBuffer(ps_constant_buffer);
    const std::array ps_constant_buffers{pixel_shader_constant_buffer_.Get()};
    device_context.PSSetConstantBuffers(1, ps_constant_buffers.size(), ps_constant_buffers.data());

    const std::array shader_resources{texture_.Get()};
    device_context.PSSetShaderResources(1, shader_resources.size(), shader_resources.data());

    const std::array samplers{texture_sampler_state_.Get()};
    device_context.PSSetSamplers(1, samplers.size(), samplers.data());

    const std::array vertex_buffers = {vertex_buffer_.Get()};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> strides{sizeof(Vertex)};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> offsets{};
    device_context.IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());
    device_context.IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

    D3D11_BUFFER_DESC index_buffer_desc;
    index_buffer_->GetDesc(&index_buffer_desc);
    const std::uint32_t index_count = index_buffer_desc.ByteWidth / sizeof(Index);
    device_context.DrawIndexed(index_count, 0, 0);
}

void TriangleComponent::InitializeVertexShader() {
    const std::string shadow_map_cascade_count_definition = std::to_string(Game::shadow_map_cascade_count);
    const std::array vertex_shader_defines{
        D3D_SHADER_MACRO{
            .Name = Game::shadow_map_cascade_count_name.data(),
            .Definition = shadow_map_cascade_count_definition.c_str(),
        },
        D3D_SHADER_MACRO{},
    };
    vertex_shader_byte_code_ = detail::ShaderFromFile(
        "resources/shaders/triangle_component.hlsl", vertex_shader_defines.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VSMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(vertex_shader_byte_code_->GetBufferPointer(),
                                                       vertex_shader_byte_code_->GetBufferSize(), nullptr,
                                                       &vertex_shader_);
    detail::CheckResult(result, "Failed to create vertex shader from byte code");
}

void TriangleComponent::InitializeVertexShaderConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(VertexShaderConstantBuffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &vertex_shader_constant_buffer_);
    detail::CheckResult(result, "Failed to create vertex shader constant buffer");
}

void TriangleComponent::InitializePixelShader() {
    const std::string shadow_map_cascade_count_definition = std::to_string(Game::shadow_map_cascade_count);
    const std::array pixel_shader_defines{
        D3D_SHADER_MACRO{
            .Name = Game::shadow_map_cascade_count_name.data(),
            .Definition = shadow_map_cascade_count_definition.c_str(),
        },
        D3D_SHADER_MACRO{},
    };
    pixel_shader_byte_code_ = detail::ShaderFromFile(
        "resources/shaders/triangle_component.hlsl", pixel_shader_defines.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreatePixelShader(
        pixel_shader_byte_code_->GetBufferPointer(), pixel_shader_byte_code_->GetBufferSize(), nullptr, &pixel_shader_);
    detail::CheckResult(result, "Failed to create pixel shader from byte code");
}

void TriangleComponent::InitializePixelShaderConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(PixelShaderConstantBuffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &pixel_shader_constant_buffer_);
    detail::CheckResult(result, "Failed to create pixel shader constant buffer");
}

void TriangleComponent::InitializeShadowMapVertexShader() {
    const std::string shadow_map_cascade_count_definition = std::to_string(Game::shadow_map_cascade_count);
    const std::array shadow_map_geometry_shader_defines{
        D3D_SHADER_MACRO{
            .Name = Game::shadow_map_cascade_count_name.data(),
            .Definition = shadow_map_cascade_count_definition.c_str(),
        },
        D3D_SHADER_MACRO{},
    };
    shadow_map_vertex_shader_byte_code_ = detail::ShaderFromFile(
        "resources/shaders/triangle_component_shadow_map.hlsl", shadow_map_geometry_shader_defines.data(),
        D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(shadow_map_vertex_shader_byte_code_->GetBufferPointer(),
                                                       shadow_map_vertex_shader_byte_code_->GetBufferSize(), nullptr,
                                                       &shadow_map_vertex_shader_);
    detail::CheckResult(result, "Failed to create shadow map vertex shader from byte code");
}

void TriangleComponent::InitializeShadowMapRasterizerState() {
    const D3D11_RASTERIZER_DESC shadow_map_rasterizer_desc{
        .FillMode = wireframe_ ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID, .CullMode = D3D11_CULL_BACK,
        // .DepthBias = 1,
        // .DepthBiasClamp = 0.0f,
        // .SlopeScaledDepthBias = 2.0f,
    };

    const HRESULT result = Device().CreateRasterizerState(&shadow_map_rasterizer_desc, &shadow_map_rasterizer_state_);
    detail::CheckResult(result, "Failed to create rasterizer state");
}

void TriangleComponent::InitializeInputLayout() {
    std::array input_elements = std::to_array(Vertex::InputElements);
    input_elements[0].SemanticName = "POSITION";

    const HRESULT result = Device().CreateInputLayout(input_elements.data(), input_elements.size(),
                                                      vertex_shader_byte_code_->GetBufferPointer(),
                                                      vertex_shader_byte_code_->GetBufferSize(), &input_layout_);
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

void TriangleComponent::InitializeTextureSamplerState() {
    constexpr D3D11_SAMPLER_DESC sampler_desc{
        .Filter = D3D11_FILTER_ANISOTROPIC,
        .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
        .ComparisonFunc = D3D11_COMPARISON_ALWAYS,
        .BorderColor = {1.0f, 0.0f, 0.0f, 1.0f},
        .MaxLOD = D3D11_FLOAT32_MAX,
    };

    const HRESULT result = Device().CreateSamplerState(&sampler_desc, &texture_sampler_state_);
    detail::CheckResult(result, "Failed to create sampler state");
}

void TriangleComponent::InitializeVertexBuffer(const std::span<const Vertex> vertices) {
    if (vertices.empty()) {
        vertex_buffer_ = nullptr;
        return;
    }

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

void TriangleComponent::InitializeIndexBuffer(const std::span<const Index> indices) {
    if (indices.empty()) {
        index_buffer_ = nullptr;
        return;
    }

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

void TriangleComponent::UpdateVertexShaderConstantBuffer(const VertexShaderConstantBuffer &data) {
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    const HRESULT result =
        DeviceContext().Map(vertex_shader_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    detail::CheckResult(result, "Failed to map constant buffer data");

    std::memcpy(mapped_subresource.pData, &data, sizeof(data));
    DeviceContext().Unmap(vertex_shader_constant_buffer_.Get(), 0);
}

void TriangleComponent::UpdatePixelShaderConstantBuffer(const PixelShaderConstantBuffer &data) {
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    const HRESULT result =
        DeviceContext().Map(pixel_shader_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    detail::CheckResult(result, "Failed to map constant buffer data");

    std::memcpy(mapped_subresource.pData, &data, sizeof(data));
    DeviceContext().Unmap(pixel_shader_constant_buffer_.Get(), 0);
}

}  // namespace borov_engine
