#include "borov_engine/debug_draw.hpp"

#include <d3dcompiler.h>

#include <array>
#include <execution>
#include <span>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"

namespace borov_engine {

constexpr std::size_t DebugDraw::max_points_count = 1024 * 1024;

DebugDraw::Vertex::Vertex(const math::Vector3& position, const math::Color& color, const float duration)
    : VertexPositionColor(position, color), duration{duration} {}

DebugDraw::DebugDraw(class Game& game, const Initializer& initializer)
    : Component(game, initializer), should_update_primitive_vertex_buffer_{} {
    InitializeConstantBuffer();

    InitializePrimitiveVertexShader();
    InitializePrimitivePixelShader();
    InitializePrimitiveInputLayout();
    InitializePrimitiveRasterizerState();
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
    std::array input_elements = std::to_array(Vertex::InputElements);
    input_elements[0].SemanticName = "POSITION";

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

void DebugDraw::InitializePrimitiveVertexBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = max_points_count * sizeof(Vertex),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &primitive_vertex_buffer_);
    detail::CheckResult(result, "Failed to create primitive vertex buffer");
}

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
        .view = (camera != nullptr) ? camera->ViewMatrix() : math::Matrix4x4::Identity,
        .projection = (camera != nullptr) ? camera->ProjectionMatrix() : math::Matrix4x4::Identity,
    };
    DeviceContext().UpdateSubresource(constant_buffer_.Get(), 0, nullptr, &constant_buffer, 0, 0);

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
    // meshes.clear();
}

void DebugDraw::Draw(const Camera* camera) {
    DrawPrimitives(camera);
    RemoveOldPrimitives();
    // DrawMeshes();
}

void DebugDraw::RemoveOldPrimitives() {
    std::erase_if(primitive_vertices_, [](const Vertex& vertex) { return vertex.duration <= 0.0f; });
}

void DebugDraw::DrawLine(const math::Vector3& start, const math::Vector3& end, const DrawOpts& opts) {
    primitive_vertices_.emplace_back(start, opts.color, opts.duration);
    primitive_vertices_.emplace_back(end, opts.color, opts.duration);

    should_update_primitive_vertex_buffer_ = true;
}

void DebugDraw::DrawBox(const math::Box& box, const DrawOpts& opts) {
    const auto corners = math::Corners(box);

    DrawLine(corners[0], corners[1], opts);
    DrawLine(corners[1], corners[2], opts);
    DrawLine(corners[2], corners[3], opts);
    DrawLine(corners[3], corners[0], opts);

    DrawLine(corners[4], corners[5], opts);
    DrawLine(corners[5], corners[6], opts);
    DrawLine(corners[6], corners[7], opts);
    DrawLine(corners[7], corners[4], opts);

    DrawLine(corners[0], corners[4], opts);
    DrawLine(corners[1], corners[5], opts);
    DrawLine(corners[2], corners[6], opts);
    DrawLine(corners[3], corners[7], opts);
}

void DebugDraw::DrawAxisAlignedBox(const math::AxisAlignedBox& box, const DrawOpts& opts) {
    const auto corners = math::Corners(box);

    DrawLine(corners[0], corners[1], opts);
    DrawLine(corners[1], corners[2], opts);
    DrawLine(corners[2], corners[3], opts);
    DrawLine(corners[3], corners[0], opts);

    DrawLine(corners[4], corners[5], opts);
    DrawLine(corners[5], corners[6], opts);
    DrawLine(corners[6], corners[7], opts);
    DrawLine(corners[7], corners[4], opts);

    DrawLine(corners[0], corners[4], opts);
    DrawLine(corners[1], corners[5], opts);
    DrawLine(corners[2], corners[6], opts);
    DrawLine(corners[3], corners[7], opts);
}

void DebugDraw::DrawArrow(const math::Vector3& start, const math::Vector3& end, const math::Vector3& normal,
                          const DrawOpts& opts) {
    DrawLine(start, end, opts);

    const math::Vector3 a = math::Vector3::Lerp(start, end, 0.85f);
    const math::Vector3 side = normal.Cross(end - start) * 0.05f;
    DrawLine(a + side, end, opts);
    DrawLine(a - side, end, opts);
}

void DebugDraw::DrawPivot(const Transform& transform, const DrawOpts& opts) {
    const auto& [position, rotation, scale] = transform;

    const math::Vector3 x_delta = math::Vector3::Transform({scale.x, 0.0f, 0.0f}, rotation);
    const math::Vector3 x_start = position - x_delta;
    const math::Vector3 x_end = position + x_delta;
    const math::Vector3 x_normal = math::Vector3::Transform(math::Vector3::Forward, rotation);
    const math::Color x_color{math::colors::linear::Red};
    DrawArrow(x_start, x_end, x_normal, {x_color * opts.color, opts.duration});

    const math::Vector3 y_delta = math::Vector3::Transform({0.0f, scale.y, 0.0f}, rotation);
    const math::Vector3 y_start = position - y_delta;
    const math::Vector3 y_end = position + y_delta;
    const math::Vector3 y_normal = math::Vector3::Transform(math::Vector3::Forward, rotation);
    const math::Color y_color{math::colors::linear::Lime};
    DrawArrow(y_start, y_end, y_normal, {y_color * opts.color, opts.duration});

    const math::Vector3 z_delta = math::Vector3::Transform({0.0f, 0.0f, scale.z}, rotation);
    const math::Vector3 z_start = position - z_delta;
    const math::Vector3 z_end = position + z_delta;
    const math::Vector3 z_normal = math::Vector3::Transform(math::Vector3::Up, rotation);
    const math::Color z_color{math::colors::linear::Blue};
    DrawArrow(z_start, z_end, z_normal, {z_color * opts.color, opts.duration});
}

void DebugDraw::DrawEllipsis(const Transform& transform, const EllipsisDrawOpts& opts) {
    const float angle_step = std::numbers::pi_v<float> * 2.0f / static_cast<float>(opts.density);

    const auto& [position, rotation, scale] = transform;
    for (decltype(opts.density) i = 0; i < opts.density; i++) {
        const auto step = static_cast<float>(i);
        math::Vector3 start{
            scale.x * std::cos(angle_step * step),
            scale.y * std::sin(angle_step * step),
            0.0f,
        };
        math::Vector3 end{
            scale.x * std::cos(angle_step * (step + 1.0f)),
            scale.y * std::sin(angle_step * (step + 1.0f)),
            0.0f,
        };

        start = math::Vector3::Transform(start, rotation) + position;
        end = math::Vector3::Transform(end, rotation) + position;
        DrawLine(start, end, {opts.color, opts.duration});
    }
}

void DebugDraw::DrawCircle(const math::Vector3& position, const float radius, const EllipsisDrawOpts& opts) {
    const Transform transform{
        .position = position,
        .scale = math::Vector3::One * radius,
    };
    DrawEllipsis(transform, opts);
}

void DebugDraw::DrawEllipsoid(const Transform& transform, const EllipsisDrawOpts& opts) {
    constexpr float angle = std::numbers::pi_v<float> / 2.0f;

    DrawEllipsis(transform, opts);

    const math::Quaternion xz_rotation = math::Quaternion::CreateFromYawPitchRoll(angle, 0.0f, 0.0f);
    const Transform xz_transform{
        .position = transform.position,
        .rotation = math::Quaternion::Concatenate(xz_rotation, transform.rotation),
        .scale = math::Vector3::Transform(transform.scale, xz_rotation),
    };
    DrawEllipsis(xz_transform, opts);

    const math::Quaternion yz_rotation = math::Quaternion::CreateFromYawPitchRoll(0.0f, angle, 0.0f);
    const Transform yz_transform{
        .position = transform.position,
        .rotation = math::Quaternion::Concatenate(yz_rotation, transform.rotation),
        .scale = math::Vector3::Transform(transform.scale, yz_rotation),
    };
    DrawEllipsis(yz_transform, opts);
}

void DebugDraw::DrawSphere(const math::Sphere& sphere, const EllipsisDrawOpts& opts) {
    const Transform transform{
        .position = sphere.Center,
        .scale = math::Vector3::One * sphere.Radius,
    };
    DrawEllipsoid(transform, opts);
}

void DebugDraw::DrawPlane(const math::Plane& plane, const PlaneDrawOpts& opts) {
    math::Vector3 normal = plane.Normal();
    if (normal.LengthSquared() == 0.0f) {
        return;
    }
    normal.Normalize();

    math::Vector3 up = math::Vector3::Backward;
    math::Vector3 right = normal.Cross(up);
    if (right.LengthSquared() < 0.1f) {
        up = math::Vector3::Up;
        right = normal.Cross(up);
    }
    right.Normalize();

    up = right.Cross(normal);

    const math::Vector3 position = -normal * plane.D();

    const math::Vector3 right_point = position + right * opts.width;
    const math::Vector3 left_point = position - right * opts.width;
    const math::Vector3 up_point = position + up * opts.width;
    const math::Vector3 down_point = position - up * opts.width;

    const DrawOpts draw_opts = {opts.color, opts.duration};
    DrawLine(left_point + up * opts.width, right_point + up * opts.width, draw_opts);
    DrawLine(left_point - up * opts.width, right_point - up * opts.width, draw_opts);
    DrawLine(down_point - right * opts.width, up_point - right * opts.width, draw_opts);
    DrawLine(down_point + right * opts.width, up_point + right * opts.width, draw_opts);

    if (opts.draw_center_cross) {
        DrawLine(left_point, right_point, draw_opts);
        DrawLine(down_point, up_point, draw_opts);
    }
    if (opts.draw_position) {
        DrawPivot({.position = position, .scale = math::Vector3::One * 0.5f}, draw_opts);
    }
    if (opts.draw_normal) {
        DrawArrow(position, position + normal * opts.length_of_normal, right, draw_opts);
    }
}

void DebugDraw::DrawFrustrum(const math::Frustum& frustum, const DrawOpts& opts) {
    const Transform pivot_transform{
        .position = frustum.Origin,
        .rotation = frustum.Orientation,
    };
    DrawPivot(pivot_transform, opts);

    const auto corners = math::Corners(frustum);

    const math::Color blue{math::colors::linear::Blue};
    DrawLine(corners[0], corners[1], {blue * opts.color, opts.duration});
    DrawLine(corners[2], corners[3], {blue * opts.color, opts.duration});
    DrawLine(corners[4], corners[5], {blue * opts.color, opts.duration});
    DrawLine(corners[6], corners[7], {blue * opts.color, opts.duration});

    const math::Color green{math::colors::linear::Lime};
    const math::Color dark_green{math::colors::linear::Green};
    DrawLine(corners[0], corners[2], {green * opts.color, opts.duration});
    DrawLine(corners[1], corners[3], {dark_green * opts.color, opts.duration});
    DrawLine(corners[4], corners[6], {green * opts.color, opts.duration});
    DrawLine(corners[5], corners[7], {dark_green * opts.color, opts.duration});

    const math::Color red{math::colors::linear::Red};
    const math::Color dark_red{math::colors::linear::DarkRed};
    DrawLine(corners[0], corners[4], {red * opts.color, opts.duration});
    DrawLine(corners[1], corners[5], {dark_red * opts.color, opts.duration});
    DrawLine(corners[2], corners[6], {red * opts.color, opts.duration});
    DrawLine(corners[3], corners[7], {dark_red * opts.color, opts.duration});
}

void DebugDraw::Update(const float delta_time) {
    Component::Update(delta_time);

    auto decrease_duration = [delta_time](Vertex& vertex) { vertex.duration -= delta_time; };
    std::for_each(std::execution::par, primitive_vertices_.begin(), primitive_vertices_.end(), decrease_duration);
}

// void DebugDraw::DrawStaticMesh(const StaticMesh& mesh, const DirectX::SimpleMath::Matrix& transform,
//                                const DirectX::SimpleMath::Color& color) {
//     MeshInfo meshInfo = {&mesh, color.ToVector4(), transform};
//
//     meshes.emplace_back(meshInfo);
// }

}  // namespace borov_engine
