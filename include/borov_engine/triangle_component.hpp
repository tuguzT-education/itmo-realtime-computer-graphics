#pragma once

#ifndef BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED

#include "component.hpp"

#include <SimpleMath.h>
#include <VertexTypes.h>

#include <span>

#include "borov_engine/detail/d3d_ptr.hpp"

namespace borov_engine {

class TriangleComponent : public Component {
  public:
    using Vertex = DirectX::VertexPositionColor;
    using Index = int;
    using Offset = DirectX::SimpleMath::Vector3;

    explicit TriangleComponent(Game &game,
                               std::span<Vertex> vertices,
                               std::span<Index> indices);

    [[nodiscard]] const Offset &GetOffset() const;
    [[nodiscard]] Offset &GetOffset();

    void Update(float delta_time) override;
    void Draw() override;

  private:
    void InitializeVertexShader();
    void InitializeIndexShader();
    void InitializeInputLayout();
    void InitializeRasterizerState();
    void InitializeVertexBuffer(std::span<Vertex> vertices);
    void InitializeIndexBuffer(std::span<Index> indices);
    void InitializeConstantBuffer(Offset offset);

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
    detail::D3DPtr<ID3D11InputLayout> input_layout_;

    detail::D3DPtr<ID3D11Buffer> constant_buffer_;
    Offset offset_;

    detail::D3DPtr<ID3D11Buffer> index_buffer_;
    detail::D3DPtr<ID3D11PixelShader> index_shader_;
    detail::D3DPtr<ID3DBlob> index_byte_code_;

    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;
    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_byte_code_;
};

}

#endif //BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
