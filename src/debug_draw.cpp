#include "borov_engine/debug_draw.hpp"

#include <d3dcompiler.h>

#include <array>
#include <span>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"

namespace borov_engine {

constexpr std::size_t DebugDraw::MaxPointsCount = 1024 * 1024;

DebugDraw::Vertex::Vertex(const math::Vector3& position, const math::Color& color, const float duration)
    : VertexPositionColor(position, color), duration{duration} {}

DebugDraw::DebugDraw(class Game& game, const Initializer& initializer)
    : Component(game, initializer), should_update_primitive_vertex_buffer_{} {
    InitializeConstantBuffer();

    InitializePrimitiveVertexShader();
    InitializePrimitivePixelShader();
    InitializePrimitiveInputLayout();
    InitializePrimitiveRasterizerState();
    InitializePrimitiveDepthState();
    InitializePrimitiveVertexBuffer();

    // InitQuads();
    // InitMeshes();
}

void DebugDraw::InitializeConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(ConstantBuffer),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &constant_buffer_);
    detail::CheckResult(result, "Failed to create constant buffer");
}

void DebugDraw::InitializePrimitiveVertexShader() {
    primitive_vertex_byte_code_ = detail::ShaderFromFile("resources/shaders/debug_draw_primitive.hlsl", nullptr,
                                                         D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0",
                                                         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);

    const HRESULT result = Device().CreateVertexShader(primitive_vertex_byte_code_->GetBufferPointer(),
                                                       primitive_vertex_byte_code_->GetBufferSize(), nullptr,
                                                       &primitive_vertex_shader_);
    detail::CheckResult(result, "Failed to create primitive vertex shader from byte code");
}

void DebugDraw::InitializePrimitivePixelShader() {
    primitive_pixel_byte_code_ = detail::ShaderFromFile("resources/shaders/debug_draw_primitive.hlsl", nullptr,
                                                        D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0",
                                                        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0);
    const HRESULT result = Device().CreatePixelShader(primitive_pixel_byte_code_->GetBufferPointer(),
                                                      primitive_pixel_byte_code_->GetBufferSize(), nullptr,
                                                      &primitive_pixel_shader_);
    detail::CheckResult(result, "Failed to create primitive pixel shader from byte code");
}

void DebugDraw::InitializePrimitiveInputLayout() {
    const std::array input_elements = std::to_array(Vertex::InputElements);
    const HRESULT result = Device().CreateInputLayout(
        input_elements.data(), input_elements.size(), primitive_vertex_byte_code_->GetBufferPointer(),
        primitive_vertex_byte_code_->GetBufferSize(), &primitive_input_layout_);
    detail::CheckResult(result, "Failed to create primitive input layout");
}

void DebugDraw::InitializePrimitiveRasterizerState() {
    constexpr D3D11_RASTERIZER_DESC rasterizer_desc{
        .FillMode = D3D11_FILL_WIREFRAME,
        .CullMode = D3D11_CULL_NONE,
    };

    const HRESULT result = Device().CreateRasterizerState(&rasterizer_desc, &primitive_rasterizer_state_);
    detail::CheckResult(result, "Failed to create primitive rasterizer state");
}

void DebugDraw::InitializePrimitiveDepthState() {
    constexpr D3D11_DEPTH_STENCIL_DESC depthDesc{
        .DepthEnable = false,
    };

    const HRESULT result = Device().CreateDepthStencilState(&depthDesc, &depthState);
    detail::CheckResult(result, "Failed to create primitive depth state");
}

void DebugDraw::InitializePrimitiveVertexBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = MaxPointsCount * sizeof(Vertex),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &primitive_vertex_buffer_);
    detail::CheckResult(result, "Failed to create primitive vertex buffer");
}

// void DebugDraw::InitQuads() {
//     quadProjMatrix = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(game->Display->ClientWidth),
//                                                          static_cast<float>(game->Display->ClientHeight), 0, 0.1f,
//                                                          1000.0f);
//
//     ID3DBlob* errorCode = nullptr;
//
//     D3DCompileFromFile(L"Shaders/TexturedShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0",
//                        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &vertexQuadCompResult, &errorCode);
//     game->Device->CreateVertexShader(vertexQuadCompResult->GetBufferPointer(), vertexQuadCompResult->GetBufferSize(),
//                                      nullptr, &vertexQuadShader);
//
//     if (errorCode) errorCode->Release();
//
//     D3DCompileFromFile(L"Shaders/TexturedShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0",
//                        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &pixelQuadCompResult, &errorCode);
//     game->Device->CreatePixelShader(pixelQuadCompResult->GetBufferPointer(), pixelQuadCompResult->GetBufferSize(),
//                                     nullptr, &pixelQuadShader);
//
//     if (errorCode) errorCode->Release();
//
//     quadLayout = VertexPositionTex::GetLayout(vertexQuadCompResult);
//     quadBindingStride = VertexPositionTex::Stride;
//
//     quads.reserve(10);
//
//     auto points = new Vector4[8]{
//         Vector4(1, 1, 0.0f, 1.0f),       Vector4(1.0f, 1.0f, 0.0f, 0.0f), Vector4(0, 1, 0.0f, 1.0f),
//         Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(1, 0, 0.0f, 1.0f),       Vector4(1.0f, 0.0f, 0.0f, 0.0f),
//         Vector4(0, 0, 0.0f, 1.0f),       Vector4(0.0f, 0.0f, 0.0f, 0.0f),
//     };
//
//     D3D11_BUFFER_DESC bufDesc = {};
//     {
//         bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//         bufDesc.CPUAccessFlags = 0;
//         bufDesc.MiscFlags = 0;
//         bufDesc.Usage = D3D11_USAGE_DEFAULT;
//         bufDesc.ByteWidth = sizeof(float) * 4 * 8;
//     }
//
//     D3D11_SUBRESOURCE_DATA subData = {};
//     subData.pSysMem = points;
//
//     game->Device->CreateBuffer(&bufDesc, &subData, &quadBuf);
//
//     delete[] points;
//
//     float borderCol[] = {1.0f, 0.0f, 0.0f, 1.0f};
//
//     D3D11_SAMPLER_DESC samplDesc = {};
//     {
//         samplDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
//         samplDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
//         samplDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
//         samplDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//         samplDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//         samplDesc.BorderColor[0] = 1.0f;
//         samplDesc.BorderColor[1] = 0.0f;
//         samplDesc.BorderColor[2] = 0.0f;
//         samplDesc.BorderColor[3] = 1.0f;
//         samplDesc.MaxLOD = static_cast<float>(INT_MAX);
//     }
//     game->Device->CreateSamplerState(&samplDesc, &quadSampler);
//
//     D3D11_RASTERIZER_DESC rastDesc = {};
//     {
//         rastDesc.CullMode = D3D11_CULL_NONE;
//         rastDesc.FillMode = D3D11_FILL_SOLID;
//     }
//
//     game->Device->CreateRasterizerState(&rastDesc, &quadRastState);
// }
//
// void DebugDraw::InitMeshes() {
//     ID3DBlob* errorCode;
//
//     D3DCompileFromFile(L"Shaders/Simple.hlsl", nullptr, nullptr, "VSMainMesh", "vs_5_0",
//                        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &vertexMeshCompResult, &errorCode);
//     game->Device->CreateVertexShader(vertexMeshCompResult->GetBufferPointer(), vertexMeshCompResult->GetBufferSize(),
//                                      nullptr, &vertexMeshShader);
//
//     if (errorCode) errorCode->Release();
//
//     D3DCompileFromFile(L"Shaders/Simple.hlsl", nullptr, nullptr, "PSMain", "ps_5_0",
//     D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
//                        0, &pixelMeshCompResult, &errorCode);
//     game->Device->CreatePixelShader(pixelMeshCompResult->GetBufferPointer(), pixelMeshCompResult->GetBufferSize(),
//                                     nullptr, &pixelMeshShader);
//
//     if (errorCode) errorCode->Release();
//
//     meshLayout = VertexPositionNormalBinormalTangentColorTex::GetLayout(vertexMeshCompResult);
//
//     D3D11_BUFFER_DESC bufDesc = {};
//     {
//         bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//         bufDesc.CPUAccessFlags = 0;
//         bufDesc.MiscFlags = 0;
//         bufDesc.Usage = D3D11_USAGE_DEFAULT;
//         bufDesc.ByteWidth = sizeof(MeshConstData);
//     }
//
//     game->Device->CreateBuffer(&bufDesc, nullptr, &meshBuf);
// }

void DebugDraw::DrawPrimitives(const Camera* camera) {
    if (should_update_primitive_vertex_buffer_) {
        UpdatePrimitiveVertexBuffer();
        should_update_primitive_vertex_buffer_ = false;
    }

    const ConstantBuffer constant_buffer{
        .view = (camera != nullptr) ? camera->View() : math::Matrix4x4::Identity,
        .projection = (camera != nullptr) ? camera->Projection() : math::Matrix4x4::Identity,
    };
    DeviceContext().UpdateSubresource(constant_buffer_.Get(), 0, nullptr, &constant_buffer, 0, 0);

    DeviceContext().OMSetDepthStencilState(depthState.Get(), 0);
    DeviceContext().RSSetState(primitive_rasterizer_state_.Get());

    DeviceContext().VSSetShader(primitive_vertex_shader_.Get(), nullptr, 0);
    DeviceContext().PSSetShader(primitive_pixel_shader_.Get(), nullptr, 0);

    DeviceContext().IASetInputLayout(primitive_input_layout_.Get());
    DeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    const std::array vertex_buffers = {primitive_vertex_buffer_.Get()};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> strides{sizeof(Vertex)};
    constexpr std::array<std::uint32_t, vertex_buffers.size()> offsets{};
    DeviceContext().IASetVertexBuffers(0, vertex_buffers.size(), vertex_buffers.data(), strides.data(), offsets.data());

    const std::array constant_buffers{constant_buffer_.Get()};
    DeviceContext().VSSetConstantBuffers(0, constant_buffers.size(), constant_buffers.data());

    DeviceContext().Draw(primitive_vertices_.size(), 0);
}

// void DebugDraw::DrawQuads() {
//     if (quads.empty()) return;
//
//     game->Context->OMSetDepthStencilState(depthState, 0);
//     game->Context->RSSetState(quadRastState);
//
//     game->Context->VSSetShader(vertexQuadShader, nullptr, 0);
//     game->Context->PSSetShader(pixelQuadShader, nullptr, 0);
//
//     game->Context->IASetInputLayout(quadLayout);
//     game->Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//     const UINT offset = 0;
//     game->Context->IASetVertexBuffers(0, 1, &quadBuf, &quadBindingStride, &offset);
//
//     game->Context->VSSetConstantBuffers(0, 1, &constBuf);
//
//     for (auto& quad : quads) {
//         auto mat = quad.TransformMat * quadProjMatrix;
//         game->Context->UpdateSubresource(constBuf, 0, nullptr, &mat, 0, 0);
//
//         game->Context->PSSetShaderResources(0, 1, &quad.Srv);
//         game->Context->PSSetSamplers(0, 1, &quadSampler);
//
//         game->Context->Draw(4, 0);
//     }
// }
//
// void DebugDraw::DrawMeshes() {
//     if (meshes.empty()) return;
//
//     game->Context->OMSetDepthStencilState(depthState, 0);
//     game->Context->RSSetState(rastState);
//
//     game->Context->VSSetShader(vertexMeshShader, nullptr, 0);
//     game->Context->PSSetShader(pixelMeshShader, nullptr, 0);
//
//     game->Context->IASetInputLayout(meshLayout);
//     game->Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//     game->Context->VSSetConstantBuffers(1, 1, &meshBuf);
//
//     const UINT offset = 0;
//     for (auto& mesh : meshes) {
//         game->Context->IASetVertexBuffers(0, 1, &mesh.Mesh->VertexBuffer, &mesh.Mesh->Stride, &offset);
//         game->Context->IASetIndexBuffer(mesh.Mesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//         MeshConstData data = MeshConstData{mesh.Transform * camera->ViewMatrix * camera->ProjMatrix, mesh.Color};
//
//         game->Context->UpdateSubresource(meshBuf, 0, nullptr, &data, 0, 0);
//
//         game->Context->DrawIndexed(mesh.Mesh->IndexCount, 0, 0);
//     }
// }

void DebugDraw::UpdatePrimitiveVertexBuffer() {
    D3D11_MAPPED_SUBRESOURCE subresource{};
    const HRESULT result =
        DeviceContext().Map(primitive_vertex_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
    detail::CheckResult(result, "Failed to map primitive vertex buffer");

    const std::span primitive_vertex_data{primitive_vertices_};
    memcpy(subresource.pData, primitive_vertex_data.data(), primitive_vertex_data.size_bytes());

    DeviceContext().Unmap(primitive_vertex_buffer_.Get(), 0);
}

void DebugDraw::Clear() {
    primitive_vertices_.clear();
    // quads.clear();
    // meshes.clear();
}

void DebugDraw::Draw(const Camera* camera) {
    DrawPrimitives(camera);
    // DrawQuads();
    // DrawMeshes();
}

void DebugDraw::DrawLine(const math::Vector3& start, const math::Vector3& end, const math::Color& color) {
    primitive_vertices_.emplace_back(start, color, 0.0f);
    primitive_vertices_.emplace_back(end, color, 0.0f);

    should_update_primitive_vertex_buffer_ = true;
}

void DebugDraw::DrawBox(const math::Box& box, const math::Color& color) {
    std::array<math::Vector3, 8> corners;
    box.GetCorners(corners.data());

    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);

    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);

    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}

void DebugDraw::DrawAxisAlignedBox(const math::AxisAlignedBox& box, const math::Color& color) {
    std::array<math::Vector3, 8> corners;
    box.GetCorners(corners.data());

    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);

    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);

    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}

void DebugDraw::DrawArrow(const math::Vector3& start, const math::Vector3& end, const math::Color& color,
                          const math::Vector3& normal) {
    DrawLine(start, end, color);

    const math::Vector3 a = math::Vector3::Lerp(start, end, 0.85f);
    const math::Vector3 side = normal.Cross(end - start) * 0.05f;
    DrawLine(a + side, end, color);
    DrawLine(a - side, end, color);
}

void DebugDraw::DrawPivot(const math::Vector3& position, const float size, const math::Color& color) {
    const auto& [x, y, z] = position;

    const math::Color red{math::colors::linear::Red};
    DrawArrow({x + size, y, z}, {x - size, y, z}, red * color, math::Vector3::Forward);

    const math::Color green{math::colors::linear::Lime};
    DrawArrow({x, y + size, z}, {x, y - size, z}, green * color, math::Vector3::Right);

    const math::Color blue{math::colors::linear::Blue};
    DrawArrow({x, y, z + size}, {x, y, z - size}, blue * color, math::Vector3::Up);
}

void DebugDraw::DrawCircle(const float radius, const math::Color& color, const math::Matrix4x4& transform,
                           const std::uint8_t density) {
    const float angle_step = std::numbers::pi_v<float> * 2.0f / static_cast<float>(density);

    for (std::uint8_t i = 0; i < density; i++) {
        const auto step = static_cast<float>(i);
        math::Vector3 start{
            radius * std::cos(angle_step * step),
            radius * std::sin(angle_step * step),
            0.0f,
        };
        math::Vector3 end{
            radius * std::cos(angle_step * (step + 1.0f)),
            radius * std::sin(angle_step * (step + 1.0f)),
            0.0f,
        };

        start = math::Vector3::Transform(start, transform);
        end = math::Vector3::Transform(end, transform);
        DrawLine(start, end, color);
    }
}

void DebugDraw::DrawSphere(const float radius, const math::Color& color, const math::Matrix4x4& transform,
                           const std::uint8_t density) {
    constexpr float angle = std::numbers::pi_v<float> / 2.0f;

    DrawCircle(radius, color, transform, density);
    DrawCircle(radius, color, math::Matrix4x4::CreateRotationX(angle) * transform, density);
    DrawCircle(radius, color, math::Matrix4x4::CreateRotationY(angle) * transform, density);
}

void DebugDraw::DrawPlane(const math::Plane& plane, const math::Color& color, const float width,
                          const float normal_length, const bool drawCenterCross) {
    math::Vector3 normal = plane.Normal();
    if (normal.Length() == 0.0f) {
        return;
    }
    normal.Normalize();

    math::Vector3 up = math::Vector3::Backward;
    math::Vector3 right = normal.Cross(up);
    if (right.Length() < 0.01f) {
        up = math::Vector3(0, 1, 0);
        right = normal.Cross(up);
    }
    right.Normalize();

    up = right.Cross(normal);

    const math::Vector3 position = -normal * plane.D();

    const math::Vector3 right_point = position + right * width;
    const math::Vector3 left_point = position - right * width;
    const math::Vector3 up_point = position + up * width;
    const math::Vector3 down_point = position - up * width;

    DrawLine(left_point + up * width, right_point + up * width, color);
    DrawLine(left_point - up * width, right_point - up * width, color);
    DrawLine(down_point - right * width, up_point - right * width, color);
    DrawLine(down_point + right * width, up_point + right * width, color);

    if (drawCenterCross) {
        DrawLine(left_point, right_point, color);
        DrawLine(down_point, up_point, color);
    }

    DrawPivot(position, 0.5f, color);
    DrawArrow(position, position + normal * normal_length, color, right);
}

void DebugDraw::DrawFrustrum(const math::Frustum& frustum) {
    std::array<math::Vector3, 8> corners;
    frustum.GetCorners(corners.data());

    DrawPivot(frustum.Origin, 1.0f, math::colors::linear::White.v);

    DrawLine(corners[0], corners[1], math::Color(0.0f, 0.0f, 1.0f, 1.0f));
    DrawLine(corners[2], corners[3], math::Color(0.0f, 0.0f, 1.0f, 1.0f));
    DrawLine(corners[4], corners[5], math::Color(0.0f, 0.0f, 1.0f, 1.0f));
    DrawLine(corners[6], corners[7], math::Color(0.0f, 0.0f, 1.0f, 1.0f));

    DrawLine(corners[0], corners[2], math::Color(0.0f, 1.0f, 0.0f, 1.0f));
    DrawLine(corners[1], corners[3], math::Color(0.0f, 0.5f, 0.0f, 1.0f));
    DrawLine(corners[4], corners[6], math::Color(0.0f, 1.0f, 0.0f, 1.0f));
    DrawLine(corners[5], corners[7], math::Color(0.0f, 0.5f, 0.0f, 1.0f));

    DrawLine(corners[0], corners[4], math::Color(1.0f, 0.0f, 0.0f, 1.0f));
    DrawLine(corners[1], corners[5], math::Color(0.5f, 0.0f, 0.0f, 1.0f));
    DrawLine(corners[2], corners[6], math::Color(1.0f, 0.0f, 0.0f, 1.0f));
    DrawLine(corners[3], corners[7], math::Color(0.5f, 0.0f, 0.0f, 1.0f));
}

// void DebugDraw::DrawTextureOnScreen(ID3D11ShaderResourceView* tex, int x, int y, int width, int height, int zOrder) {
//     if (quads.size() >= QuadMaxDrawCount) return;
//
//     QuadInfo quad = {};
//     quad.Srv = tex;
//     quad.TransformMat =
//         Matrix::CreateScale(static_cast<float>(width), static_cast<float>(height), 1.0f) *
//         Matrix::CreateTranslation(static_cast<float>(x), static_cast<float>(y), static_cast<float>(zOrder));
//
//     quads.emplace_back(quad);
// }

// void DebugDraw::DrawStaticMesh(const StaticMesh& mesh, const DirectX::SimpleMath::Matrix& transform,
//                                const DirectX::SimpleMath::Color& color) {
//     MeshInfo meshInfo = {&mesh, color.ToVector4(), transform};
//
//     meshes.emplace_back(meshInfo);
// }

}  // namespace borov_engine
