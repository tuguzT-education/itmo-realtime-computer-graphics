#pragma once

#ifndef BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED

#include "component.hpp"

#include <VertexTypes.h>

#include <span>

#include "transform.hpp"
#include "detail/d3d_ptr.hpp"

namespace borov_engine {

class TriangleComponent : public Component {
  public:
    using Vertex = DirectX::VertexPositionColor;
    using Index = std::uint32_t;

    explicit TriangleComponent(borov_engine::Game &game,
                               std::span<Vertex> vertices,
                               std::span<Index> indices,
                               borov_engine::Transform transform = {});

    [[nodiscard]] const borov_engine::Transform &Transform() const;
    [[nodiscard]] borov_engine::Transform &Transform();

    void Update(float delta_time) override;
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

    borov_engine::Transform transform_;

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

}

#endif //BOROV_ENGINE_TRIANGLE_COMPONENT_HPP_INCLUDED
