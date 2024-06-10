#pragma once

#ifndef BOROV_ENGINE_TEXTURE_DRAW_HPP_INCLUDED
#define BOROV_ENGINE_TEXTURE_DRAW_HPP_INCLUDED

#include <VertexTypes.h>

#include "component.hpp"
#include "detail/d3d_ptr.hpp"
#include "math.hpp"

namespace borov_engine {

class TextureDraw : public Component {
  public:
    using Vertex = DirectX::VertexPositionTexture;

    explicit TextureDraw(class Game& game, const Initializer& initializer = {});

    void Clear();

    virtual void DrawTexture(const detail::D3DPtr<ID3D11ShaderResourceView>& texture, int x, int y, int width,
                             int height, int z_order);

    void Draw(const Camera* camera) override;
    void OnTargetResize() override;

  private:
    struct ConstantBuffer {
        alignas(16) math::Matrix4x4 world;
        alignas(16) math::Matrix4x4 view;
        alignas(16) math::Matrix4x4 projection;
    };

    void InitializeProjectionMatrix();
    void InitializeVertexShader();
    void InitializePixelShader();
    void InitializeInputLayout();
    void InitializeVertexBuffer();
    void InitializeSamplerState();
    void InitializeRasterizerState();
    void InitializeConstantBuffer();

    void DrawTextures();

    detail::D3DPtr<ID3D11Buffer> constant_buffer_;

    math::Matrix4x4 projection_matrix_;

    struct TextureInfo {
        detail::D3DPtr<ID3D11ShaderResourceView> texture;
        math::Matrix4x4 transform_matrix;
    };

    std::vector<TextureInfo> textures_;

    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;

    detail::D3DPtr<ID3D11PixelShader> pixel_shader_;
    detail::D3DPtr<ID3DBlob> pixel_shader_byte_code_;

    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_shader_byte_code_;

    detail::D3DPtr<ID3D11InputLayout> input_layout_;
    detail::D3DPtr<ID3D11SamplerState> sampler_state_;

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_TEXTURE_DRAW_HPP_INCLUDED
