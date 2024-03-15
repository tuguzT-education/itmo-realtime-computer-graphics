#pragma once

#ifndef BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED

#include <GeometricPrimitive.h>

#include <variant>

#include "component.hpp"
#include "transform.hpp"

namespace borov_engine {

using GeometricPrimitive = DirectX::GeometricPrimitive;

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

struct BoxGeometricPrimitiveArguments {
    math::Vector3 size = math::Vector3::One;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

struct SphereGeometricPrimitiveArguments {
    float diameter = 1.0;
    std::size_t tessellation = 16;
    bool right_handed_coords = true;
    bool invert_normals = false;
};

struct GeoSphereGeometricPrimitiveArguments {
    float diameter = 1.0f;
    std::size_t tessellation = 3;
    bool right_handed_coords = true;
};

struct CylinderGeometricPrimitiveArguments {
    float height = 1.0f;
    float diameter = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

struct ConeGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float height = 1.0f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

struct TorusGeometricPrimitiveArguments {
    float diameter = 1.0f;
    float thickness = 0.333f;
    std::size_t tessellation = 32;
    bool right_handed_coords = true;
};

struct TetrahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

struct OctahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

struct DodecahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

struct IcosahedronGeometricPrimitiveArguments {
    float size = 1.0f;
    bool right_handed_coords = true;
};

struct TeapotGeometricPrimitiveArguments {
    float size = 1.0f;
    std::size_t tessellation = 8;
    bool right_handed_coords = true;
};

struct CustomGeometricPrimitiveArguments {
    GeometricPrimitive::VertexCollection vertices;
    GeometricPrimitive::IndexCollection indices;
};

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

    void Update(float delta_time) override;
    void Draw(const Camera *camera) override;

  private:
    std::unique_ptr<GeometricPrimitive> CreatePrimitive(const GeometricPrimitiveArguments &custom_arguments);

    borov_engine::Transform transform_;
    math::Color color_;
    bool wireframe_;
    std::unique_ptr<GeometricPrimitive> primitive_;
    GeometricPrimitiveArguments primitive_arguments_;
};

}

#endif //BOROV_ENGINE_GEOMETRIC_PRIMITIVE_COMPONENT_HPP_INCLUDED
