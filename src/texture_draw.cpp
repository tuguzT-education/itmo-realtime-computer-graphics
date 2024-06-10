#include "borov_engine/texture_draw.hpp"

#include <d3dcompiler.h>

#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

const std::array points{
    TextureDraw::Vertex{math::Vector3{1.0f, 1.0f, 0.0f}, math::Vector2{1.0f, 1.0f}},
    TextureDraw::Vertex{math::Vector3{0.0f, 1.0f, 0.0f}, math::Vector2{0.0f, 1.0f}},
    TextureDraw::Vertex{math::Vector3{1.0f, 0.0f, 0.0f}, math::Vector2{1.0f, 0.0f}},
    TextureDraw::Vertex{math::Vector3{0.0f, 0.0f, 0.0f}, math::Vector2{0.0f, 0.0f}},
};

TextureDraw::TextureDraw(class Game& game, const Initializer& initializer) : Component(game, initializer) {
    InitializeProjectionMatrix();
    InitializeVertexShader();
    InitializePixelShader();
    InitializeVertexBuffer();
    InitializeInputLayout();
    InitializeSamplerState();
    InitializeRasterizerState();
    InitializeConstantBuffer();
}

void TextureDraw::Clear() {
    textures_.clear();
}

void TextureDraw::DrawTexture(const detail::D3DPtr<ID3D11ShaderResourceView>& texture, const int x, const int y,
                              const int width, const int height, const int z_order) {
    const math::Matrix4x4 transform_matrix =
        math::Matrix4x4::CreateScale(static_cast<float>(width), static_cast<float>(height), 1.0f) *
        math::Matrix4x4::CreateTranslation(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z_order));

    textures_.emplace_back(texture, transform_matrix);
}

void TextureDraw::Draw(const Camera* camera) {
    Component::Draw(camera);

    DrawTextures();
}

void TextureDraw::OnTargetResize() {
    Component::OnTargetResize();

    InitializeProjectionMatrix();
}

void TextureDraw::InitializeProjectionMatrix() {
    const auto width = static_cast<float>(Game().TargetWidth());
    const auto height = static_cast<float>(Game().TargetHeight());
    projection_matrix_ = math::Matrix4x4::CreateOrthographicOffCenter(0, width, height, 0, 0.1f, 1000.0f);
}

void TextureDraw::InitializeVertexShader() {
    vertex_shader_byte_code_ = detail::ShaderFromFile("resources/shaders/texture_draw.hlsl", nullptr,
                                                      D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0",
                                                      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(vertex_shader_byte_code_->GetBufferPointer(),
                                                       vertex_shader_byte_code_->GetBufferSize(), nullptr,
                                                       &vertex_shader_);
    detail::CheckResult(result, "Failed to create texture draw vertex shader");
}

void TextureDraw::InitializePixelShader() {
    pixel_shader_byte_code_ = detail::ShaderFromFile("resources/shaders/texture_draw.hlsl", nullptr,
                                                     D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0",
                                                     D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreatePixelShader(
        pixel_shader_byte_code_->GetBufferPointer(), pixel_shader_byte_code_->GetBufferSize(), nullptr, &pixel_shader_);
    detail::CheckResult(result, "Failed to create texture draw pixel shader");
}

void TextureDraw::InitializeInputLayout() {
    std::array input_elements = std::to_array(Vertex::InputElements);
    input_elements[0].SemanticName = "POSITION";

    const HRESULT result = Device().CreateInputLayout(input_elements.data(), input_elements.size(),
                                                      vertex_shader_byte_code_->GetBufferPointer(),
                                                      vertex_shader_byte_code_->GetBufferSize(), &input_layout_);
    detail::CheckResult(result, "Failed to create texture draw input layout");
}

void TextureDraw::InitializeVertexBuffer() {
    constexpr D3D11_BUFFER_DESC vertex_buffer_desc{
        .ByteWidth = static_cast<UINT>(std::span{points}.size_bytes()),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_VERTEX_BUFFER,
    };
    constexpr D3D11_SUBRESOURCE_DATA initial_data{
        .pSysMem = points.data(),
    };

    const HRESULT result = Device().CreateBuffer(&vertex_buffer_desc, &initial_data, &vertex_buffer_);
    detail::CheckResult(result, "Failed to create texture draw vertex buffer");
}

void TextureDraw::InitializeSamplerState() {
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
    detail::CheckResult(result, "Failed to create texture draw sampler state");
}

void TextureDraw::InitializeRasterizerState() {
    constexpr D3D11_RASTERIZER_DESC rasterizer_desc{
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_NONE,
    };

    const HRESULT result = Device().CreateRasterizerState(&rasterizer_desc, &rasterizer_state_);
    detail::CheckResult(result, "Failed to create texture draw rasterizer state");
}

void TextureDraw::InitializeConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(ConstantBuffer),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &constant_buffer_);
    detail::CheckResult(result, "Failed to create texture draw constant buffer");
}

void TextureDraw::DrawTextures() {
    DeviceContext().RSSetState(rasterizer_state_.Get());
    DeviceContext().IASetInputLayout(input_layout_.Get());
    DeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    constexpr std::array<ID3D11ClassInstance*, 0> vs_class_instances;
    DeviceContext().VSSetShader(vertex_shader_.Get(), vs_class_instances.data(), vs_class_instances.size());

    constexpr std::array<ID3D11ClassInstance*, 0> ps_class_instances;
    DeviceContext().PSSetShader(pixel_shader_.Get(), ps_class_instances.data(), ps_class_instances.size());

    const std::array vertex_buffers{vertex_buffer_.Get()};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> strides{sizeof(Vertex)};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> offsets{};
    DeviceContext().IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());

    const std::array constant_buffers{constant_buffer_.Get()};
    DeviceContext().VSSetConstantBuffers(0, constant_buffers.size(), constant_buffers.data());

    for (const auto& [shader_resource_view, transform_matrix] : textures_) {
        const math::Matrix4x4 matrix = transform_matrix * projection_matrix_;
        DeviceContext().UpdateSubresource(constant_buffer_.Get(), 0, nullptr, &matrix, 0, 0);

        DeviceContext().PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
        DeviceContext().PSSetSamplers(0, 1, &sampler_state_);

        DeviceContext().Draw(points.size(), 0);
    }
}

}  // namespace borov_engine
