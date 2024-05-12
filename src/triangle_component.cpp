#include "borov_engine/triangle_component.hpp"

#include <WICTextureLoader.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <d3dcompiler.h>

#undef min
#undef max

#include <array>
#include <assimp/Importer.hpp>
#include <range/v3/view/enumerate.hpp>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/check_result.hpp"
#include "borov_engine/detail/shader.hpp"
#include "borov_engine/detail/texture.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

namespace detail {

Transform TransformFromNode(const aiNode &node) {
    aiVector3D position, scale;
    aiQuaternion rotation;
    node.mTransformation.Decompose(scale, rotation, position);

    return Transform{
        .position = math::Vector3{position.x, position.y, position.z},
        .rotation = math::Quaternion{rotation.x, rotation.y, rotation.z, rotation.w},
        .scale = math::Vector3{scale.x, scale.y, scale.z},
    };
}

TriangleComponent &TriangleFromMesh(Game &game, const SceneComponent &parent, const aiScene &scene, const aiMesh &mesh,
                                    const std::filesystem::path &mesh_path) {
    math::Color diffuse{math::colors::linear::White};
    std::filesystem::path diffuse_texture_path;
    if (const aiMaterial *material = scene.mNumMaterials > 0 ? scene.mMaterials[mesh.mMaterialIndex] : nullptr) {
        if (aiColor3D ai_diffuse{1.0f, 1.0f, 1.0f};
            material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_diffuse) == aiReturn_SUCCESS) {
            diffuse = math::Color{ai_diffuse.r, ai_diffuse.g, ai_diffuse.b};
        }
        if (aiString ai_texture_diffuse;
            material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), ai_texture_diffuse) == aiReturn_SUCCESS) {
            diffuse_texture_path = mesh_path;
            diffuse_texture_path.remove_filename();
            diffuse_texture_path += std::string_view{ai_texture_diffuse.data, ai_texture_diffuse.length};
        }
    }

    std::vector<TriangleComponent::Vertex> vertices;
    for (const std::span ai_vertices{mesh.mVertices, mesh.mNumVertices};
         const auto &[index, ai_position] : ranges::views::enumerate(ai_vertices)) {
        const auto [x, y, z] = ai_position;
        const math::Vector3 position{x, y, z};

        math::Color color{math::colors::linear::White};
        if (const aiColor4D *colors = mesh.mColors[0]) {
            const auto [r, g, b, a] = colors[index];
            color = math::Color{r, g, b, a} * diffuse;
        }

        math::Vector2 texture_coordinate;
        if (const aiVector3D *texture_coordinates = mesh.mTextureCoords[0]) {
            const auto [x, y, z] = texture_coordinates[index];
            texture_coordinate = math::Vector2{x, y};
        }

        vertices.emplace_back(position, color, texture_coordinate);
    }

    std::vector<TriangleComponent::Index> indices;
    for (const std::span faces{mesh.mFaces, mesh.mNumFaces}; const aiFace &face : faces) {
        for (const std::span ai_indices{face.mIndices, face.mNumIndices};
             const std::uint32_t index : ai_indices | std::views::take(3)) {
            indices.emplace_back(index);
        }
    }

    TriangleComponent::CustomInitializer initializer{
        .vertices = vertices,
        .indices = indices,
        .texture_path = diffuse_texture_path,
    };
    initializer.Parent(&parent);
    return game.AddComponent<TriangleComponent>(initializer);
}

void TraverseNode(Game &game, const SceneComponent &parent, const aiScene &scene, const aiNode &node,
                  const std::filesystem::path &mesh_path) {
    const SceneComponent::Initializer root_args{.transform = TransformFromNode(node), .parent = &parent};
    const auto &root = game.AddComponent<SceneComponent>(root_args);

    for (const std::size_t mesh_index : std::span{node.mMeshes, node.mNumMeshes}) {
        if (const aiMesh *mesh = scene.mMeshes[mesh_index]) {
            TriangleFromMesh(game, root, scene, *mesh, mesh_path);
        }
    }

    for (const aiNode *child_node : std::span{node.mChildren, node.mNumChildren}) {
        TraverseNode(game, root, scene, *child_node, mesh_path);
    }
}

}  // namespace detail

auto TriangleComponent::Initializer::Wireframe(const bool wireframe) -> Initializer & {
    this->wireframe = wireframe;
    return *this;
}

auto TriangleComponent::CustomInitializer::Vertices(const std::span<const Vertex> vertices) -> Initializer & {
    this->vertices = vertices;
    return *this;
}

auto TriangleComponent::CustomInitializer::Indices(const std::span<const Index> indices) -> Initializer & {
    this->indices = indices;
    return *this;
}

auto TriangleComponent::CustomInitializer::TexturePath(const std::filesystem::path &texture_path) -> Initializer & {
    this->texture_path = texture_path;
    return *this;
}

auto TriangleComponent::CustomInitializer::TileCount(const math::Vector2 tile_count) -> Initializer & {
    this->tile_count = tile_count;
    return *this;
}

auto TriangleComponent::MeshInitializer::MeshPath(const std::filesystem::path &mesh_path) -> Initializer & {
    this->mesh_path = mesh_path;
    return *this;
}

TriangleComponent::TriangleComponent(class Game &game, const Initializer &initializer)
    : SceneComponent(game, initializer), wireframe_{initializer.wireframe}, tile_count_{math::Vector2::One} {
    InitializeVertexShader();
    InitializeIndexShader();
    InitializeInputLayout();
    InitializeRasterizerState();
    InitializeSamplerState();
    InitializeConstantBuffer();
}

TriangleComponent::TriangleComponent(class Game &game, const CustomInitializer &initializer)
    : TriangleComponent(game, static_cast<const Initializer &>(initializer)) {
    Load(initializer.vertices, initializer.indices);
    LoadTexture(initializer.texture_path, initializer.tile_count);
}

TriangleComponent::TriangleComponent(class Game &game, const MeshInitializer &initializer)
    : TriangleComponent(game, static_cast<const Initializer &>(initializer)) {
    LoadMesh(initializer.mesh_path);
}

void TriangleComponent::Load(const std::span<const Vertex> vertices, const std::span<const Index> indices) {
    InitializeVertexBuffer(vertices);
    InitializeIndexBuffer(indices);
}

void TriangleComponent::LoadTexture(const std::filesystem::path &texture_path, const math::Vector2 tile_count) {
    if (!texture_path.has_filename()) {
        return;
    }

    texture_ = detail::TextureFromFile(Device(), texture_path);
    tile_count_ = tile_count;
}

void TriangleComponent::LoadMesh(const std::filesystem::path &mesh_path) {
    if (!mesh_path.has_filename()) {
        return;
    }
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(mesh_path.generic_string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (scene == nullptr) {
        const char *message = importer.GetErrorString();
        throw std::runtime_error{message};
    }

    if (const aiNode *node = scene->mRootNode) {
        detail::TraverseNode(Game(), *this, *scene, *node, mesh_path);
    }
}

bool TriangleComponent::Wireframe() const {
    return wireframe_;
}

void TriangleComponent::Wireframe(const bool wireframe) {
    wireframe_ = wireframe;
    InitializeRasterizerState();
}

void TriangleComponent::Draw(const Camera *camera) {
    if (vertex_buffer_ == nullptr || index_buffer_ == nullptr) {
        return;
    }

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
        .tile_count = tile_count_,
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
        .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
        .ComparisonFunc = D3D11_COMPARISON_ALWAYS,
        .BorderColor = {1.0f, 0.0f, 0.0f, 1.0f},
        .MaxLOD = D3D11_FLOAT32_MAX,
    };

    const HRESULT result = Device().CreateSamplerState(&sampler_desc, &sampler_state_);
    detail::CheckResult(result, "Failed to create sampler state");
}

void TriangleComponent::InitializeVertexBuffer(std::span<const Vertex> vertices) {
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

void TriangleComponent::InitializeIndexBuffer(std::span<const Index> indices) {
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

void TriangleComponent::InitializeConstantBuffer() {
    constexpr D3D11_BUFFER_DESC buffer_desc{
        .ByteWidth = sizeof(ConstantBuffer),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    const HRESULT result = Device().CreateBuffer(&buffer_desc, nullptr, &constant_buffer_);
    detail::CheckResult(result, "Failed to create constant buffer");
}

}  // namespace borov_engine
