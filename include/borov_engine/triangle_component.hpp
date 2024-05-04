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

    explicit TriangleComponent(class Game &game, std::span<const Vertex> vertices = {},
                               std::span<const Index> indices = {}, const std::filesystem::path &texture_path = {},
                               bool wireframe = false, const class Transform &transform = {},
                               const SceneComponent *parent = nullptr);

    void Load(std::span<const Vertex> vertices, std::span<const Index> indices);
    void LoadTexture(const std::filesystem::path &texture_path);
    void LoadMesh(const std::filesystem::path &mesh_path);

    [[nodiscard]] bool Wireframe() const;
    void Wireframe(bool wireframe);

    void Draw(const Camera *camera) override;

  private:
    struct ConstantBuffer {
        alignas(16) math::Matrix4x4 world;
        alignas(16) math::Matrix4x4 view;
        alignas(16) math::Matrix4x4 projection;
        std::uint32_t has_texture = false;
    };

    void InitializeVertexShader();
    void InitializeIndexShader();
    void InitializeInputLayout();
    void InitializeRasterizerState();
    void InitializeSamplerState();
    void InitializeVertexBuffer(std::span<const Vertex> vertices);
    void InitializeIndexBuffer(std::span<const Index> indices);
    void InitializeConstantBuffer(ConstantBuffer constant_buffer);

    detail::D3DPtr<ID3D11SamplerState> sampler_state_;
    detail::D3DPtr<ID3D11ShaderResourceView> texture_;

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
    detail::D3DPtr<ID3D11InputLayout> input_layout_;

    detail::D3DPtr<ID3D11Buffer> constant_buffer_;

    detail::D3DPtr<ID3D11Buffer> index_buffer_;
    detail::D3DPtr<ID3D11PixelShader> index_shader_;
    detail::D3DPtr<ID3DBlob> index_byte_code_;

    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;
    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_byte_code_;

    bool wireframe_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
