#pragma once

#ifndef BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED

#include <VertexTypes.h>

#include <filesystem>

#include "detail/d3d_ptr.hpp"
#include "scene_component.hpp"

namespace borov_engine {

class TriangleComponent : public SceneComponent {
  public:
    using Vertex = DirectX::VertexPositionColorTexture;
    using Index = std::uint32_t;

    struct Initializer : SceneComponent::Initializer {
        bool wireframe = false;

        Initializer &Wireframe(bool wireframe);
    };

    struct CustomInitializer : Initializer {
        std::span<const Vertex> vertices;
        std::span<const Index> indices;
        std::filesystem::path texture_path;
        math::Vector2 tile_count = math::Vector2::One;

        Initializer &Vertices(std::span<const Vertex> vertices);
        Initializer &Indices(std::span<const Index> indices);
        Initializer &TexturePath(const std::filesystem::path &texture_path);
        Initializer &TileCount(math::Vector2 tile_count);
    };

    struct MeshInitializer : Initializer {
        std::filesystem::path mesh_path;

        Initializer &MeshPath(const std::filesystem::path &mesh_path);
    };

    explicit TriangleComponent(class Game &game, const Initializer &initializer = {});
    explicit TriangleComponent(class Game &game, const CustomInitializer &initializer);
    explicit TriangleComponent(class Game &game, const MeshInitializer &initializer);

    void Load(std::span<const Vertex> vertices, std::span<const Index> indices);
    void LoadTexture(const std::filesystem::path &texture_path, math::Vector2 tile_count = math::Vector2::One);
    void LoadMesh(const std::filesystem::path &mesh_path);

    [[nodiscard]] bool Wireframe() const;
    void Wireframe(bool wireframe);

    void Draw(const Camera *camera) override;

  private:
    struct alignas(16) VertexShaderConstantBuffer {
        alignas(16) math::Matrix4x4 world;
        alignas(16) math::Matrix4x4 view;
        alignas(16) math::Matrix4x4 projection;
        math::Vector2 tile_count = math::Vector2::One;
    };

    struct alignas(16) PixelShaderConstantBuffer {
        std::uint32_t has_texture = false;
    };

    void InitializeVertexShader();
    void InitializeVertexShaderConstantBuffer();

    void InitializePixelShader();
    void InitializePixelShaderConstantBuffer();

    void InitializeInputLayout();
    void InitializeRasterizerState();
    void InitializeSamplerState();

    void InitializeVertexBuffer(std::span<const Vertex> vertices);
    void InitializeIndexBuffer(std::span<const Index> indices);

    void UpdateVertexShaderConstantBuffer(const VertexShaderConstantBuffer &data);
    void UpdatePixelShaderConstantBuffer(const PixelShaderConstantBuffer &data);

    detail::D3DPtr<ID3D11Buffer> index_buffer_;
    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;

    detail::D3DPtr<ID3D11SamplerState> sampler_state_;
    detail::D3DPtr<ID3D11ShaderResourceView> texture_;

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
    detail::D3DPtr<ID3D11InputLayout> input_layout_;

    detail::D3DPtr<ID3D11Buffer> pixel_shader_constant_buffer_;
    detail::D3DPtr<ID3D11PixelShader> pixel_shader_;
    detail::D3DPtr<ID3DBlob> pixel_byte_code_;

    detail::D3DPtr<ID3D11Buffer> vertex_shader_constant_buffer_;
    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_byte_code_;

    bool wireframe_;
    math::Vector2 tile_count_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
