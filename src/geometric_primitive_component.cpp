#include "borov_engine/geometric_primitive_component.hpp"

namespace borov_engine {

namespace detail {

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

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
      primitive_{CreatePrimitive(primitive_arguments)},
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
    primitive_ = CreatePrimitive(primitive_arguments);
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

void GeometricPrimitiveComponent::Update(float delta_time) {}

void GeometricPrimitiveComponent::Draw(const Camera *camera) {
    if (primitive_ == nullptr) {
        return;
    }

    math::Matrix4x4 world = transform_.World();
    math::Matrix4x4 view = (camera != nullptr) ? camera->View() : math::Matrix4x4::Identity;
    math::Matrix4x4 projection = (camera != nullptr) ? camera->Projection() : math::Matrix4x4::Identity;

    primitive_->Draw(world, view, projection, color_, nullptr, wireframe_);
}

std::unique_ptr<GeometricPrimitive> GeometricPrimitiveComponent::CreatePrimitive(const GeometricPrimitiveArguments &primitive_arguments) {
    auto device_context = &DeviceContext();
    return std::visit(detail::overloaded{
        [device_context](const CubeGeometricPrimitiveArguments &cube_arguments) {
            return GeometricPrimitive::CreateCube(device_context,
                                                  cube_arguments.size,
                                                  cube_arguments.right_handed_coords);
        },
        [device_context](const BoxGeometricPrimitiveArguments &box_arguments) {
            return GeometricPrimitive::CreateBox(device_context,
                                                 box_arguments.size,
                                                 box_arguments.right_handed_coords,
                                                 box_arguments.invert_normals);
        },
        [device_context](const SphereGeometricPrimitiveArguments &sphere_arguments) {
            return GeometricPrimitive::CreateSphere(device_context,
                                                    sphere_arguments.diameter,
                                                    sphere_arguments.tessellation,
                                                    sphere_arguments.right_handed_coords,
                                                    sphere_arguments.invert_normals);
        },
        [device_context](const GeoSphereGeometricPrimitiveArguments &geo_sphere_arguments) {
            return GeometricPrimitive::CreateGeoSphere(device_context,
                                                       geo_sphere_arguments.diameter,
                                                       geo_sphere_arguments.tessellation,
                                                       geo_sphere_arguments.right_handed_coords);
        },
        [device_context](const CylinderGeometricPrimitiveArguments &cylinder_arguments) {
            return GeometricPrimitive::CreateCylinder(device_context,
                                                      cylinder_arguments.height,
                                                      cylinder_arguments.diameter,
                                                      cylinder_arguments.tessellation,
                                                      cylinder_arguments.right_handed_coords);
        },
        [device_context](const ConeGeometricPrimitiveArguments &cone_arguments) {
            return GeometricPrimitive::CreateCone(device_context,
                                                  cone_arguments.diameter,
                                                  cone_arguments.height,
                                                  cone_arguments.tessellation,
                                                  cone_arguments.right_handed_coords);
        },
        [device_context](const TorusGeometricPrimitiveArguments &torus_arguments) {
            return GeometricPrimitive::CreateTorus(device_context,
                                                   torus_arguments.diameter,
                                                   torus_arguments.thickness,
                                                   torus_arguments.tessellation,
                                                   torus_arguments.right_handed_coords);
        },
        [device_context](const TetrahedronGeometricPrimitiveArguments &tetrahedron_arguments) {
            return GeometricPrimitive::CreateTetrahedron(device_context,
                                                         tetrahedron_arguments.size,
                                                         tetrahedron_arguments.right_handed_coords);
        },
        [device_context](const OctahedronGeometricPrimitiveArguments &octahedron_arguments) {
            return GeometricPrimitive::CreateOctahedron(device_context,
                                                        octahedron_arguments.size,
                                                        octahedron_arguments.right_handed_coords);
        },
        [device_context](const DodecahedronGeometricPrimitiveArguments &dodecahedron_arguments) {
            return GeometricPrimitive::CreateDodecahedron(device_context,
                                                          dodecahedron_arguments.size,
                                                          dodecahedron_arguments.right_handed_coords);
        },
        [device_context](const IcosahedronGeometricPrimitiveArguments &icosahedron_arguments) {
            return GeometricPrimitive::CreateIcosahedron(device_context,
                                                         icosahedron_arguments.size,
                                                         icosahedron_arguments.right_handed_coords);
        },
        [device_context](const TeapotGeometricPrimitiveArguments &teapot_arguments) {
            return GeometricPrimitive::CreateTeapot(device_context,
                                                    teapot_arguments.size,
                                                    teapot_arguments.tessellation,
                                                    teapot_arguments.right_handed_coords);
        },
        [device_context](const CustomGeometricPrimitiveArguments &custom_arguments) {
            return GeometricPrimitive::CreateCustom(device_context,
                                                    custom_arguments.vertices,
                                                    custom_arguments.indices);
        },
    }, primitive_arguments);
}

}
