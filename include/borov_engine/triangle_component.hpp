#pragma once

#ifndef BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED

#include <VertexTypes.h>

#include <span>

#include "detail/d3d_ptr.hpp"
#include "scene_component.hpp"

namespace borov_engine {

class TriangleComponent : public SceneComponent {
  public:
    using Vertex = DirectX::VertexPositionColor;
    using Index = std::uint32_t;

    explicit TriangleComponent(class Game &game, std::span<Vertex> vertices, std::span<Index> indices,
                               const class Transform &transform = {}, const SceneComponent *parent = nullptr);

    void Draw(const Camera *camera) override;

  private:
    struct ConstantBuffer {
        alignas(16) math::Matrix4x4 wvp_matrix;
    };

    void InitializeVertexShader();
    void InitializeIndexShader();
    void InitializeInputLayout();
    void InitializeRasterizerState();
    void InitializeVertexBuffer(std::span<Vertex> vertices);
    void InitializeIndexBuffer(std::span<Index> indices);
    void InitializeConstantBuffer(ConstantBuffer constant_buffer);

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
    detail::D3DPtr<ID3D11InputLayout> input_layout_;

    detail::D3DPtr<ID3D11Buffer> constant_buffer_;

    detail::D3DPtr<ID3D11Buffer> index_buffer_;
    detail::D3DPtr<ID3D11PixelShader> index_shader_;
    detail::D3DPtr<ID3DBlob> index_byte_code_;

    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;
    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_byte_code_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
