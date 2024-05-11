#pragma once

#ifndef BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED

#include <GeometricPrimitive.h>

#include <filesystem>
#include <variant>

#include "detail/d3d_ptr.hpp"
#include "scene_component.hpp"

namespace borov_engine {

using GeometricPrimitive = DirectX::GeometricPrimitive;
using UniqueGeometricPrimitive = std::unique_ptr<GeometricPrimitive>;

enum class GeometricPrimitiveType : std::uint8_t {
    Cube,
    Box,
    Sphere,
    GeoSphere,
    Cylinder,
    Cone,
    Torus,
    Tetrahedron,
    Octahedron,
    Dodecahedron,
    Icosahedron,
    Teapot,
    Custom,
};

struct CubeGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateCube(ID3D11DeviceContext *device_context,
                                    const CubeGeometricPrimitiveArguments &arguments);

struct BoxGeometricPrimitiveArguments {
    math::Vector3 size = math::Vector3::One;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

UniqueGeometricPrimitive CreateBox(ID3D11DeviceContext *device_context,
                                   const BoxGeometricPrimitiveArguments &arguments);

struct SphereGeometricPrimitiveArguments {
    float diameter = 1.0;
    std::size_t tessellation = 16;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

UniqueGeometricPrimitive CreateSphere(ID3D11DeviceContext *device_context,
                                      const SphereGeometricPrimitiveArguments &arguments);

struct GeoSphereGeometricPrimitiveArguments {
    float diameter = 1.0f;
    std::size_t tessellation = 3;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateGeoSphere(ID3D11DeviceContext *device_context,
                                         const GeoSphereGeometricPrimitiveArguments &arguments);

struct CylinderGeometricPrimitiveArguments {
    float height = 1.0f;
    float diameter = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateCylinder(ID3D11DeviceContext *device_context,
                                        const CylinderGeometricPrimitiveArguments &arguments);

struct ConeGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float height = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateCone(ID3D11DeviceContext *device_context,
                                    const ConeGeometricPrimitiveArguments &arguments);

struct TorusGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float thickness = 0.333f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateTorus(ID3D11DeviceContext *device_context,
                                     const TorusGeometricPrimitiveArguments &arguments);

struct TetrahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateTetrahedron(ID3D11DeviceContext *device_context,
                                           const TetrahedronGeometricPrimitiveArguments &arguments);

struct OctahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateOctahedron(ID3D11DeviceContext *device_context,
                                          const OctahedronGeometricPrimitiveArguments &arguments);

struct DodecahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateDodecahedron(ID3D11DeviceContext *device_context,
                                            const DodecahedronGeometricPrimitiveArguments &arguments);

struct IcosahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateIcosahedron(ID3D11DeviceContext *device_context,
                                           const IcosahedronGeometricPrimitiveArguments &arguments);

struct TeapotGeometricPrimitiveArguments {
    float size = 1.0f;
    std::size_t tessellation = 8;
    bool right_handed_coords = true;
};

UniqueGeometricPrimitive CreateTeapot(ID3D11DeviceContext *device_context,
                                      const TeapotGeometricPrimitiveArguments &arguments);

struct CustomGeometricPrimitiveArguments {
    GeometricPrimitive::VertexCollection vertices;
    GeometricPrimitive::IndexCollection indices;
};

UniqueGeometricPrimitive CreateCustom(ID3D11DeviceContext *device_context,
                                      const CustomGeometricPrimitiveArguments &arguments);

using GeometricPrimitiveArguments =
    std::variant<CubeGeometricPrimitiveArguments, BoxGeometricPrimitiveArguments, SphereGeometricPrimitiveArguments,
                 GeoSphereGeometricPrimitiveArguments, CylinderGeometricPrimitiveArguments,
                 ConeGeometricPrimitiveArguments, TorusGeometricPrimitiveArguments,
                 TetrahedronGeometricPrimitiveArguments, OctahedronGeometricPrimitiveArguments,
                 DodecahedronGeometricPrimitiveArguments, IcosahedronGeometricPrimitiveArguments,
                 TeapotGeometricPrimitiveArguments, CustomGeometricPrimitiveArguments>;

UniqueGeometricPrimitive CreatePrimitive(ID3D11DeviceContext *device_context,
                                         const GeometricPrimitiveArguments &primitive_arguments);

GeometricPrimitiveType PrimitiveType(const GeometricPrimitiveArguments &arguments);
GeometricPrimitiveArguments PrimitiveArguments(GeometricPrimitiveType primitive_type);

class GeometricPrimitiveComponent : public SceneComponent {
  public:
    struct Initializer : SceneComponent::Initializer {
        GeometricPrimitiveArguments primitive_arguments;
        math::Color color{math::colors::linear::White};
        std::filesystem::path texture_path;
        bool wireframe = false;

        Initializer &PrimitiveArguments(const GeometricPrimitiveArguments &arguments);
        Initializer &PrimitiveType(GeometricPrimitiveType type);
        Initializer &Color(math::Color color);
        Initializer &TexturePath(const std::filesystem::path &texture_path);
        Initializer &Wireframe(bool wireframe);
    };

    explicit GeometricPrimitiveComponent(class Game &game, const Initializer &initializer);

    [[nodiscard]] const GeometricPrimitive *Primitive() const;
    [[nodiscard]] GeometricPrimitive *Primitive();
    GeometricPrimitive &Primitive(const GeometricPrimitiveArguments &arguments);
    GeometricPrimitive &Primitive(GeometricPrimitiveType primitive_type);

    void LoadTexture(const std::filesystem::path &texture_path);

    [[nodiscard]] GeometricPrimitiveType PrimitiveType() const;
    [[nodiscard]] const GeometricPrimitiveArguments &PrimitiveArguments() const;

    [[nodiscard]] const math::Color &Color() const;
    [[nodiscard]] math::Color &Color();

    [[nodiscard]] bool Wireframe() const;
    void Wireframe(bool wireframe);

    void Draw(const Camera *camera) override;

  private:
    math::Color color_;
    bool wireframe_;

    detail::D3DPtr<ID3D11ShaderResourceView> texture_;
    UniqueGeometricPrimitive primitive_;
    GeometricPrimitiveArguments primitive_arguments_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
