#pragma once

#ifndef BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED

#include <GeometricPrimitive.h>

#include <variant>

#include "component.hpp"
#include "transform.hpp"

namespace borov_engine {

using GeometricPrimitive = DirectX::GeometricPrimitive;
using HeapGeometricPrimitive = std::unique_ptr<GeometricPrimitive>;

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

HeapGeometricPrimitive CreateCube(ID3D11DeviceContext *device_context,
                                  const CubeGeometricPrimitiveArguments &arguments);

struct BoxGeometricPrimitiveArguments {
    math::Vector3 size = math::Vector3::One;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

HeapGeometricPrimitive CreateBox(ID3D11DeviceContext *device_context,
                                 const BoxGeometricPrimitiveArguments &arguments);

struct SphereGeometricPrimitiveArguments {
    float diameter = 1.0;
    std::size_t tessellation = 16;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

HeapGeometricPrimitive CreateSphere(ID3D11DeviceContext *device_context,
                                    const SphereGeometricPrimitiveArguments &arguments);

struct GeoSphereGeometricPrimitiveArguments {
    float diameter = 1.0f;
    std::size_t tessellation = 3;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateGeoSphere(ID3D11DeviceContext *device_context,
                                       const GeoSphereGeometricPrimitiveArguments &arguments);

struct CylinderGeometricPrimitiveArguments {
    float height = 1.0f;
    float diameter = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateCylinder(ID3D11DeviceContext *device_context,
                                      const CylinderGeometricPrimitiveArguments &arguments);

struct ConeGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float height = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateCone(ID3D11DeviceContext *device_context,
                                  const ConeGeometricPrimitiveArguments &arguments);

struct TorusGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float thickness = 0.333f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateTorus(ID3D11DeviceContext *device_context,
                                   const TorusGeometricPrimitiveArguments &arguments);

struct TetrahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateTetrahedron(ID3D11DeviceContext *device_context,
                                         const TetrahedronGeometricPrimitiveArguments &arguments);

struct OctahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateOctahedron(ID3D11DeviceContext *device_context,
                                        const OctahedronGeometricPrimitiveArguments &arguments);

struct DodecahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateDodecahedron(ID3D11DeviceContext *device_context,
                                          const DodecahedronGeometricPrimitiveArguments &arguments);

struct IcosahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateIcosahedron(ID3D11DeviceContext *device_context,
                                         const IcosahedronGeometricPrimitiveArguments &arguments);

struct TeapotGeometricPrimitiveArguments {
    float size = 1.0f;
    std::size_t tessellation = 8;
    bool right_handed_coords = true;
};

HeapGeometricPrimitive CreateTeapot(ID3D11DeviceContext *device_context,
                                    const TeapotGeometricPrimitiveArguments &arguments);

struct CustomGeometricPrimitiveArguments {
    GeometricPrimitive::VertexCollection vertices;
    GeometricPrimitive::IndexCollection indices;
};

HeapGeometricPrimitive CreateCustom(ID3D11DeviceContext *device_context,
                                    const CustomGeometricPrimitiveArguments &arguments);

using GeometricPrimitiveArguments = std::variant<CubeGeometricPrimitiveArguments,
                                                 BoxGeometricPrimitiveArguments,
                                                 SphereGeometricPrimitiveArguments,
                                                 GeoSphereGeometricPrimitiveArguments,
                                                 CylinderGeometricPrimitiveArguments,
                                                 ConeGeometricPrimitiveArguments,
                                                 TorusGeometricPrimitiveArguments,
                                                 TetrahedronGeometricPrimitiveArguments,
                                                 OctahedronGeometricPrimitiveArguments,
                                                 DodecahedronGeometricPrimitiveArguments,
                                                 IcosahedronGeometricPrimitiveArguments,
                                                 TeapotGeometricPrimitiveArguments,
                                                 CustomGeometricPrimitiveArguments>;

HeapGeometricPrimitive CreatePrimitive(ID3D11DeviceContext *device_context,
                                       const GeometricPrimitiveArguments &arguments);

GeometricPrimitiveType PrimitiveType(const GeometricPrimitiveArguments &primitive_arguments);
GeometricPrimitiveArguments PrimitiveArguments(GeometricPrimitiveType primitive_type);

class GeometricPrimitiveComponent : public Component {
  public:
    explicit GeometricPrimitiveComponent(borov_engine::Game &game,
                                         const GeometricPrimitiveArguments &primitive_arguments,
                                         borov_engine::Transform transform = {},
                                         math::Color color = math::colors::linear::White.v,
                                         bool wireframe = false);

    explicit GeometricPrimitiveComponent(borov_engine::Game &game,
                                         GeometricPrimitiveType primitive_type,
                                         borov_engine::Transform transform = {},
                                         math::Color color = math::colors::linear::White.v,
                                         bool wireframe = false);

    [[nodiscard]] const GeometricPrimitive *Primitive() const;
    [[nodiscard]] GeometricPrimitive *Primitive();
    GeometricPrimitive &Primitive(const GeometricPrimitiveArguments &primitive_arguments);
    GeometricPrimitive &Primitive(GeometricPrimitiveType primitive_type);

    [[nodiscard]] GeometricPrimitiveType PrimitiveType() const;
    [[nodiscard]] const GeometricPrimitiveArguments &PrimitiveArguments() const;

    [[nodiscard]] const borov_engine::Transform &Transform() const;
    [[nodiscard]] borov_engine::Transform &Transform();

    [[nodiscard]] const math::Color &Color() const;
    [[nodiscard]] math::Color &Color();

    [[nodiscard]] bool Wireframe() const;
    [[nodiscard]] bool &Wireframe();

    void Draw(const Camera *camera) override;

  private:
    borov_engine::Transform transform_;
    math::Color color_;
    bool wireframe_;
    HeapGeometricPrimitive primitive_;
    GeometricPrimitiveArguments primitive_arguments_;
};

}

#endif //BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
