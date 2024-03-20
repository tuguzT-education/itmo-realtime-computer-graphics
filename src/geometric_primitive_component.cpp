#include "borov_engine/geometric_primitive_component.hpp"

#include "borov_engine/camera.hpp"

namespace borov_engine {

namespace detail {

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

}

HeapGeometricPrimitive CreateCube(ID3D11DeviceContext *device_context,
                                  const CubeGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCube(device_context,
                                          arguments.size,
                                          arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateBox(ID3D11DeviceContext *device_context,
                                 const BoxGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateBox(device_context,
                                         arguments.size,
                                         arguments.right_handed_coords,
                                         arguments.invert_normals);
}

HeapGeometricPrimitive CreateSphere(ID3D11DeviceContext *device_context,
                                    const SphereGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateSphere(device_context,
                                            arguments.diameter,
                                            arguments.tessellation,
                                            arguments.right_handed_coords,
                                            arguments.invert_normals);
}

HeapGeometricPrimitive CreateGeoSphere(ID3D11DeviceContext *device_context,
                                       const GeoSphereGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateGeoSphere(device_context,
                                               arguments.diameter,
                                               arguments.tessellation,
                                               arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateCylinder(ID3D11DeviceContext *device_context,
                                      const CylinderGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCylinder(device_context,
                                              arguments.height,
                                              arguments.diameter,
                                              arguments.tessellation,
                                              arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateCone(ID3D11DeviceContext *device_context,
                                  const ConeGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCone(device_context,
                                          arguments.diameter,
                                          arguments.height,
                                          arguments.tessellation,
                                          arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateTorus(ID3D11DeviceContext *device_context,
                                   const TorusGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTorus(device_context,
                                           arguments.diameter,
                                           arguments.thickness,
                                           arguments.tessellation,
                                           arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateTetrahedron(ID3D11DeviceContext *device_context,
                                         const TetrahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTetrahedron(device_context,
                                                 arguments.size,
                                                 arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateOctahedron(ID3D11DeviceContext *device_context,
                                        const OctahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateOctahedron(device_context,
                                                arguments.size,
                                                arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateDodecahedron(ID3D11DeviceContext *device_context,
                                          const DodecahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateDodecahedron(device_context,
                                                  arguments.size,
                                                  arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateIcosahedron(ID3D11DeviceContext *device_context,
                                         const IcosahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateIcosahedron(device_context,
                                                 arguments.size,
                                                 arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateTeapot(ID3D11DeviceContext *device_context,
                                    const TeapotGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTeapot(device_context,
                                            arguments.size,
                                            arguments.tessellation,
                                            arguments.right_handed_coords);
}

HeapGeometricPrimitive CreateCustom(ID3D11DeviceContext *device_context,
                                    const CustomGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCustom(device_context,
                                            arguments.vertices,
                                            arguments.indices);
}

HeapGeometricPrimitive CreatePrimitive(ID3D11DeviceContext *device_context,
                                       const GeometricPrimitiveArguments &arguments) {
    auto c = device_context;
    return std::visit(detail::overloaded{
        [c](const CubeGeometricPrimitiveArguments &arguments) { return CreateCube(c, arguments); },
        [c](const BoxGeometricPrimitiveArguments &arguments) { return CreateBox(c, arguments); },
        [c](const SphereGeometricPrimitiveArguments &arguments) { return CreateSphere(c, arguments); },
        [c](const GeoSphereGeometricPrimitiveArguments &arguments) { return CreateGeoSphere(c, arguments); },
        [c](const CylinderGeometricPrimitiveArguments &arguments) { return CreateCylinder(c, arguments); },
        [c](const ConeGeometricPrimitiveArguments &arguments) { return CreateCone(c, arguments); },
        [c](const TorusGeometricPrimitiveArguments &arguments) { return CreateTorus(c, arguments); },
        [c](const TetrahedronGeometricPrimitiveArguments &arguments) { return CreateTetrahedron(c, arguments); },
        [c](const OctahedronGeometricPrimitiveArguments &arguments) { return CreateOctahedron(c, arguments); },
        [c](const DodecahedronGeometricPrimitiveArguments &arguments) { return CreateDodecahedron(c, arguments); },
        [c](const IcosahedronGeometricPrimitiveArguments &arguments) { return CreateIcosahedron(c, arguments); },
        [c](const TeapotGeometricPrimitiveArguments &arguments) { return CreateTeapot(c, arguments); },
        [c](const CustomGeometricPrimitiveArguments &arguments) { return CreateCustom(c, arguments); },
    }, arguments);
}

GeometricPrimitiveType PrimitiveType(const GeometricPrimitiveArguments &primitive_arguments) {
    return std::visit(detail::overloaded{
        [](const CubeGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Cube; },
        [](const BoxGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Box; },
        [](const SphereGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Sphere; },
        [](const GeoSphereGeometricPrimitiveArguments &) { return GeometricPrimitiveType::GeoSphere; },
        [](const CylinderGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Cylinder; },
        [](const ConeGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Cone; },
        [](const TorusGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Torus; },
        [](const TetrahedronGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Tetrahedron; },
        [](const OctahedronGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Octahedron; },
        [](const DodecahedronGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Dodecahedron; },
        [](const IcosahedronGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Icosahedron; },
        [](const TeapotGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Teapot; },
        [](const CustomGeometricPrimitiveArguments &) { return GeometricPrimitiveType::Custom; },
    }, primitive_arguments);
}

GeometricPrimitiveArguments PrimitiveArguments(GeometricPrimitiveType primitive_type) {
    switch (primitive_type) {
        case GeometricPrimitiveType::Cube: return CubeGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Box: return BoxGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Sphere: return SphereGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::GeoSphere: return GeoSphereGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Cylinder: return CylinderGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Cone: return ConeGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Torus: return TorusGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Tetrahedron: return TetrahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Octahedron: return OctahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Dodecahedron: return DodecahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Icosahedron: return IcosahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Teapot: return TeapotGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Custom: return CustomGeometricPrimitiveArguments{};
        default: return {};
    }
}

GeometricPrimitiveComponent::GeometricPrimitiveComponent(borov_engine::Game &game,
                                                         const GeometricPrimitiveArguments &primitive_arguments,
                                                         borov_engine::Transform transform,
                                                         math::Color color,
                                                         bool wireframe)
    : Component(game),
      primitive_{CreatePrimitive(&DeviceContext(), primitive_arguments)},
      transform_{transform},
      color_{color},
      wireframe_{wireframe},
      primitive_arguments_{primitive_arguments} {}

GeometricPrimitiveComponent::GeometricPrimitiveComponent(borov_engine::Game &game,
                                                         GeometricPrimitiveType primitive_type,
                                                         borov_engine::Transform transform,
                                                         math::Color color,
                                                         bool wireframe)
    : GeometricPrimitiveComponent(game,
                                  borov_engine::PrimitiveArguments(primitive_type),
                                  transform,
                                  color,
                                  wireframe) {}

const GeometricPrimitive *GeometricPrimitiveComponent::Primitive() const {
    return primitive_.get();
}

GeometricPrimitive *GeometricPrimitiveComponent::Primitive() {
    return primitive_.get();
}

GeometricPrimitive &GeometricPrimitiveComponent::Primitive(const GeometricPrimitiveArguments &primitive_arguments) {
    primitive_ = CreatePrimitive(&DeviceContext(), primitive_arguments);
    primitive_arguments_ = primitive_arguments;
    return *primitive_;
}

GeometricPrimitive &GeometricPrimitiveComponent::Primitive(GeometricPrimitiveType primitive_type) {
    GeometricPrimitiveArguments primitive_arguments = borov_engine::PrimitiveArguments(primitive_type);
    return Primitive(primitive_arguments);
}

GeometricPrimitiveType GeometricPrimitiveComponent::PrimitiveType() const {
    return borov_engine::PrimitiveType(primitive_arguments_);
}

const GeometricPrimitiveArguments &GeometricPrimitiveComponent::PrimitiveArguments() const {
    return primitive_arguments_;
}

const Transform &GeometricPrimitiveComponent::Transform() const {
    return transform_;
}

Transform &GeometricPrimitiveComponent::Transform() {
    return transform_;
}

const math::Color &GeometricPrimitiveComponent::Color() const {
    return color_;
}

math::Color &GeometricPrimitiveComponent::Color() {
    return color_;
}

bool GeometricPrimitiveComponent::Wireframe() const {
    return wireframe_;
}

bool &GeometricPrimitiveComponent::Wireframe() {
    return wireframe_;
}

void GeometricPrimitiveComponent::Draw(const Camera *camera) {
    if (primitive_ == nullptr) {
        return;
    }

    math::Matrix4x4 world = transform_.World();
    math::Matrix4x4 view = (camera != nullptr) ? camera->View() : math::Matrix4x4::Identity;
    math::Matrix4x4 projection = (camera != nullptr) ? camera->Projection() : math::Matrix4x4::Identity;

    primitive_->Draw(world, view, projection, color_, nullptr, wireframe_);
}

}
