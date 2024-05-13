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

    struct DrawOpts {
        math::Color color{math::colors::linear::Red};
        float duration = 0.0f;
    };
    void DrawLine(const math::Vector3& start, const math::Vector3& end, const DrawOpts& opts = {});
    void DrawBox(const math::Box& box, const DrawOpts& opts = {});
    void DrawAxisAlignedBox(const math::AxisAlignedBox& box, const DrawOpts& opts = {});
    void DrawArrow(const math::Vector3& start, const math::Vector3& end, const math::Vector3& normal,
                   const DrawOpts& opts = {});
    void DrawPivot(const Transform& transform, const DrawOpts& opts = {});

    struct EllipsisDrawOpts {
        math::Color color{math::colors::linear::Red};
        float duration = 0.0f;
        std::uint32_t density = 16;
    };
    void DrawEllipsis(const Transform& transform, const EllipsisDrawOpts& opts = {});
    void DrawCircle(const math::Vector3& position, float radius, const EllipsisDrawOpts& opts = {});
    void DrawEllipsoid(const Transform& transform, const EllipsisDrawOpts& opts = {});
    void DrawSphere(const math::Sphere& sphere, const EllipsisDrawOpts& opts = {});

    struct PlaneDrawOpts {
        math::Color color{math::colors::linear::Red};
        float duration = 0.0f;
        float width = 1.0f;
        float length_of_normal = 1.0f;
        bool draw_center_cross = false;
    };
    void DrawPlane(const math::Plane& plane, const PlaneDrawOpts& opts = {});

    void DrawFrustrum(const math::Frustum& frustum, const DrawOpts& opts = {});

    // virtual void DrawTextureOnScreen(ID3D11ShaderResourceView* tex, int x, int y, int width, int height, int zOrder);

    // virtual void DrawStaticMesh(const StaticMesh& mesh, const DirectX::SimpleMath::Matrix& transform,
    //                             const DirectX::SimpleMath::Color& color);

    void Update(float delta_time) override;
    void Draw(const Camera* camera) override;

  protected:
    void InitializeConstantBuffer();

    void InitializePrimitiveVertexShader();
    void InitializePrimitivePixelShader();
    void InitializePrimitiveInputLayout();
    void InitializePrimitiveRasterizerState();
    void InitializePrimitiveVertexBuffer();

    // void InitQuads();
    // void InitMeshes();

    void DrawPrimitives(const Camera* camera);
    void RemoveOldPrimitives();
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
    bool should_update_primitive_vertex_buffer_;

    detail::D3DPtr<ID3D11RasterizerState> primitive_rasterizer_state_;

    static const std::size_t max_points_count;
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
