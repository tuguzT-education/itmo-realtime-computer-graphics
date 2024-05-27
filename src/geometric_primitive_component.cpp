#include "borov_engine/geometric_primitive_component.hpp"

#include <directxtk/Effects.h>

#include "borov_engine/camera.hpp"
#include "borov_engine/detail/d3d_ptr.hpp"
#include "borov_engine/detail/texture.hpp"
#include "borov_engine/light.hpp"

namespace borov_engine {

namespace detail {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

}  // namespace detail

UniqueGeometricPrimitive CreateCube(ID3D11DeviceContext *device_context,
                                    const CubeGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCube(device_context, arguments.size, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateBox(ID3D11DeviceContext *device_context,
                                   const BoxGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateBox(device_context, arguments.size, arguments.right_handed_coords,
                                         arguments.invert_normals);
}

UniqueGeometricPrimitive CreateSphere(ID3D11DeviceContext *device_context,
                                      const SphereGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateSphere(device_context, arguments.diameter, arguments.tessellation,
                                            arguments.right_handed_coords, arguments.invert_normals);
}

UniqueGeometricPrimitive CreateGeoSphere(ID3D11DeviceContext *device_context,
                                         const GeoSphereGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateGeoSphere(device_context, arguments.diameter, arguments.tessellation,
                                               arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateCylinder(ID3D11DeviceContext *device_context,
                                        const CylinderGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCylinder(device_context, arguments.height, arguments.diameter,
                                              arguments.tessellation, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateCone(ID3D11DeviceContext *device_context,
                                    const ConeGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCone(device_context, arguments.diameter, arguments.height, arguments.tessellation,
                                          arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateTorus(ID3D11DeviceContext *device_context,
                                     const TorusGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTorus(device_context, arguments.diameter, arguments.thickness,
                                           arguments.tessellation, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateTetrahedron(ID3D11DeviceContext *device_context,
                                           const TetrahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTetrahedron(device_context, arguments.size, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateOctahedron(ID3D11DeviceContext *device_context,
                                          const OctahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateOctahedron(device_context, arguments.size, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateDodecahedron(ID3D11DeviceContext *device_context,
                                            const DodecahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateDodecahedron(device_context, arguments.size, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateIcosahedron(ID3D11DeviceContext *device_context,
                                           const IcosahedronGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateIcosahedron(device_context, arguments.size, arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateTeapot(ID3D11DeviceContext *device_context,
                                      const TeapotGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateTeapot(device_context, arguments.size, arguments.tessellation,
                                            arguments.right_handed_coords);
}

UniqueGeometricPrimitive CreateCustom(ID3D11DeviceContext *device_context,
                                      const CustomGeometricPrimitiveArguments &arguments) {
    return GeometricPrimitive::CreateCustom(device_context, arguments.vertices, arguments.indices);
}

UniqueGeometricPrimitive CreatePrimitive(ID3D11DeviceContext *device_context,
                                         const GeometricPrimitiveArguments &primitive_arguments) {
    auto c = device_context;
    return std::visit(
        detail::overloaded{
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
        },
        primitive_arguments);
}

GeometricPrimitiveType PrimitiveType(const GeometricPrimitiveArguments &arguments) {
    return std::visit(
        detail::overloaded{
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
        },
        arguments);
}

GeometricPrimitiveArguments PrimitiveArguments(const GeometricPrimitiveType primitive_type) {
    switch (primitive_type) {
        case GeometricPrimitiveType::Cube:
            return CubeGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Box:
            return BoxGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Sphere:
            return SphereGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::GeoSphere:
            return GeoSphereGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Cylinder:
            return CylinderGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Cone:
            return ConeGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Torus:
            return TorusGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Tetrahedron:
            return TetrahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Octahedron:
            return OctahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Dodecahedron:
            return DodecahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Icosahedron:
            return IcosahedronGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Teapot:
            return TeapotGeometricPrimitiveArguments{};
        case GeometricPrimitiveType::Custom:
            return CustomGeometricPrimitiveArguments{};
        default:
            return {};
    }
}

GeometricPrimitiveComponent::GeometricPrimitiveComponent(class Game &game, const Initializer &initializer)
    : SceneComponent(game, initializer),
      color_{initializer.color},
      wireframe_{initializer.wireframe},
      primitive_{CreatePrimitive(&DeviceContext(), initializer.primitive_arguments)},
      primitive_arguments_{initializer.primitive_arguments} {
    LoadTexture(initializer.texture_path);
}

const GeometricPrimitive *GeometricPrimitiveComponent::Primitive() const {
    return primitive_.get();
}

GeometricPrimitive *GeometricPrimitiveComponent::Primitive() {
    return primitive_.get();
}

GeometricPrimitive &GeometricPrimitiveComponent::Primitive(const GeometricPrimitiveArguments &arguments) {
    primitive_ = CreatePrimitive(&DeviceContext(), arguments);
    primitive_arguments_ = arguments;
    return *primitive_;
}

GeometricPrimitive &GeometricPrimitiveComponent::Primitive(GeometricPrimitiveType primitive_type) {
    const GeometricPrimitiveArguments primitive_arguments = borov_engine::PrimitiveArguments(primitive_type);
    return Primitive(primitive_arguments);
}

void GeometricPrimitiveComponent::LoadTexture(const std::filesystem::path &texture_path) {
    if (!texture_path.has_filename()) {
        return;
    }

    texture_ = detail::TextureFromFile(Device(), DeviceContext(), texture_path);
}

GeometricPrimitiveType GeometricPrimitiveComponent::PrimitiveType() const {
    return borov_engine::PrimitiveType(primitive_arguments_);
}

const GeometricPrimitiveArguments &GeometricPrimitiveComponent::PrimitiveArguments() const {
    return primitive_arguments_;
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

    const math::Matrix4x4 world = WorldTransform().ToMatrix();
    const math::Matrix4x4 view = (camera != nullptr) ? camera->ViewMatrix() : math::Matrix4x4::Identity;
    const math::Matrix4x4 projection = (camera != nullptr) ? camera->ProjectionMatrix() : math::Matrix4x4::Identity;

    const auto effect = std::make_unique<DirectX::BasicEffect>(&Device());
    effect->SetColorAndAlpha(color_);
    effect->SetMatrices(world, view, projection);

    const DirectionalLightComponent &directional_light_component = Game().DirectionalLight();
    const DirectionalLight directional_light = directional_light_component.DirectionalLight();
    effect->SetLightDiffuseColor(0, directional_light.diffuse);
    effect->SetLightSpecularColor(0, directional_light.specular);
    effect->SetLightEnabled(0, directional_light_component.LightEnabled());

    const PointLightComponent &point_light_component = Game().PointLight();
    PointLight point_light = point_light_component.PointLight();
    effect->SetLightDiffuseColor(1, point_light.diffuse);
    effect->SetLightSpecularColor(1, point_light.specular);
    effect->SetLightEnabled(1, point_light_component.LightEnabled());

    const SpotLightComponent &spot_light_component = Game().SpotLight();
    SpotLight spot_light = spot_light_component.SpotLight();
    effect->SetLightDiffuseColor(2, spot_light.diffuse);
    effect->SetLightSpecularColor(2, spot_light.specular);
    effect->SetLightEnabled(2, spot_light_component.LightEnabled());

    const math::Color ambient_color = directional_light.ambient + point_light.ambient + spot_light.ambient;
    effect->SetAmbientLightColor(ambient_color);
    effect->SetLightingEnabled(true);
    effect->SetPerPixelLighting(true);

    if (texture_ != nullptr) {
        effect->SetTexture(texture_.Get());
        effect->SetTextureEnabled(true);
    }

    detail::D3DPtr<ID3D11InputLayout> input_layout;
    primitive_->CreateInputLayout(effect.get(), input_layout.GetAddressOf());

    primitive_->Draw(effect.get(), input_layout.Get(), false, wireframe_);
}

}  // namespace borov_engine
