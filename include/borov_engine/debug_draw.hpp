#pragma once

#include <VertexTypes.h>

#include "component.hpp"
#include "detail/d3d_ptr.hpp"
#include "transform.hpp"

namespace borov_engine {

class DebugDraw : public Component {
  public:
    struct Vertex : DirectX::VertexPositionColor {
        float duration = 0.0f;

        explicit Vertex(const math::Vector3& position, const math::Color& color, float duration = 0.0f);
    };

    struct Initializer : Component::Initializer {};

    explicit DebugDraw(class Game& game, const Initializer& initializer = {});

    void Clear();

    virtual void DrawLine(const math::Vector3& start, const math::Vector3& end, const math::Color& color);
    virtual void DrawBox(const math::Box& box, const math::Color& color);
    virtual void DrawAxisAlignedBox(const math::AxisAlignedBox& box, const math::Color& color);
    virtual void DrawArrow(const math::Vector3& start, const math::Vector3& end, const math::Color& color,
                           const math::Vector3& normal);
    virtual void DrawPivot(const math::Vector3& position, float size, const math::Color& color);
    virtual void DrawCircle(float radius, const math::Color& color, const math::Matrix4x4& transform,
                            std::uint8_t density);
    virtual void DrawSphere(float radius, const math::Color& color, const math::Matrix4x4& transform,
                            std::uint8_t density);
    virtual void DrawPlane(const math::Plane& plane, const math::Color& color, float width, float normal_length,
                           bool drawCenterCross);
    virtual void DrawFrustrum(const math::Frustum& frustum);

    // virtual void DrawTextureOnScreen(ID3D11ShaderResourceView* tex, int x, int y, int width, int height, int zOrder);

    // virtual void DrawStaticMesh(const StaticMesh& mesh, const DirectX::SimpleMath::Matrix& transform,
    //                             const DirectX::SimpleMath::Color& color);

    void Draw(const Camera* camera) override;

  protected:
    void InitializeConstantBuffer();

    void InitializePrimitiveVertexShader();
    void InitializePrimitivePixelShader();
    void InitializePrimitiveInputLayout();
    void InitializePrimitiveRasterizerState();
    void InitializePrimitiveDepthState();
    void InitializePrimitiveVertexBuffer();

    // void InitQuads();
    // void InitMeshes();

    void DrawPrimitives(const Camera* camera);
    // void DrawQuads();
    // void DrawMeshes();

    void UpdatePrimitiveVertexBuffer();

  private:
    struct ConstantBuffer {
        alignas(16) math::Matrix4x4 world;
        alignas(16) math::Matrix4x4 view;
        alignas(16) math::Matrix4x4 projection;
    };

    detail::D3DPtr<ID3D11Buffer> constant_buffer_;

#pragma region Primitives
    std::vector<Vertex> primitive_vertices_;

    detail::D3DPtr<ID3D11PixelShader> primitive_pixel_shader_;
    detail::D3DPtr<ID3DBlob> primitive_pixel_byte_code_;

    detail::D3DPtr<ID3D11VertexShader> primitive_vertex_shader_;
    detail::D3DPtr<ID3DBlob> primitive_vertex_byte_code_;

    detail::D3DPtr<ID3D11InputLayout> primitive_input_layout_;
    detail::D3DPtr<ID3D11Buffer> primitive_vertex_buffer_;

    detail::D3DPtr<ID3D11RasterizerState> primitive_rasterizer_state_;
    detail::D3DPtr<ID3D11DepthStencilState> depthState;
    bool should_update_primitive_vertex_buffer_;

    static const std::size_t MaxPointsCount;
#pragma endregion Primitives

    // #pragma region Quads
    //
    //     math::Matrix4x4 quadProjMatrix;
    //
    //     struct QuadInfo {
    //         ID3D11ShaderResourceView* Srv;
    //         math::Matrix4x4 TransformMat;
    //     };
    //
    //     std::vector<QuadInfo> quads;
    //
    //     const UINT QuadMaxDrawCount = 100;
    //
    //     detail::D3DPtr<ID3D11Buffer> quadBuf;
    //     UINT quadBindingStride;
    //
    //     detail::D3DPtr<ID3D11PixelShader> pixelQuadShader;
    //     detail::D3DPtr<ID3D11VertexShader> vertexQuadShader;
    //     detail::D3DPtr<ID3DBlob> pixelQuadCompResult;
    //     detail::D3DPtr<ID3DBlob> vertexQuadCompResult;
    //
    //     detail::D3DPtr<ID3D11InputLayout> quadLayout;
    //     detail::D3DPtr<ID3D11SamplerState> quadSampler;
    //
    //     detail::D3DPtr<ID3D11RasterizerState> quadRastState;
    //
    // #pragma endregion Quads

    // #pragma region Meshes
    //
    //     struct MeshInfo {
    //         const StaticMesh* Mesh;
    //         DirectX::SimpleMath::Vector4 Color;
    //         DirectX::SimpleMath::Matrix Transform;
    //     };
    //
    //     struct MeshConstData {
    //         DirectX::SimpleMath::Matrix Transform;
    //         DirectX::SimpleMath::Vector4 Color;
    //     };
    //
    //     std::vector<MeshInfo> meshes;
    //
    //     ID3D11VertexShader* vertexMeshShader;
    //     ID3D11PixelShader* pixelMeshShader;
    //     ID3DBlob* pixelMeshCompResult;
    //     ID3DBlob* vertexMeshCompResult;
    //
    //     ID3D11InputLayout* meshLayout;
    //
    //     ID3D11Buffer* meshBuf;
    //
    // #pragma endregion Meshes
};

}  // namespace borov_engine
