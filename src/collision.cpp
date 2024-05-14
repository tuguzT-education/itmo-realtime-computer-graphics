#include "borov_engine/collision.hpp"

#include <algorithm>

namespace borov_engine {

Collision::~Collision() = default;

SphereCollision::SphereCollision(const PrimitiveType& primitive) : sphere_{primitive} {}

auto SphereCollision::Primitive() const -> const PrimitiveType& {
    return sphere_;
}

auto SphereCollision::Primitive() -> PrimitiveType& {
    return sphere_;
}

bool SphereCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return sphere->Primitive().Intersects(sphere_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(sphere_);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return box->Primitive().Intersects(sphere_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return frustum->Primitive().Intersects(sphere_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return sphere_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle->Primitive().Intersects(sphere_);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(sphere_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool SphereCollision::Intersects(const math::Ray& ray, float& dist) const {
    return ray.Intersects(sphere_, dist);
}

AxisAlignedBoxCollision::AxisAlignedBoxCollision(const PrimitiveType& primitive) : box_{primitive} {}

auto AxisAlignedBoxCollision::Primitive() const -> const PrimitiveType& {
    return box_;
}

auto AxisAlignedBoxCollision::Primitive() -> PrimitiveType& {
    return box_;
}

bool AxisAlignedBoxCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return sphere->Primitive().Intersects(box_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(box_);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return box->Primitive().Intersects(box_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return frustum->Primitive().Intersects(box_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return box_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle->Primitive().Intersects(box_);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool AxisAlignedBoxCollision::Intersects(const math::Ray& ray, float& dist) const {
    return ray.Intersects(box_, dist);
}

BoxCollision::BoxCollision(const PrimitiveType& primitive) : box_{primitive} {
    math::Quaternion orientation{box_.Orientation};
    orientation.Normalize();
    box_.Orientation = orientation;
}

auto BoxCollision::Primitive() const -> const PrimitiveType& {
    return box_;
}

auto BoxCollision::Primitive() -> PrimitiveType& {
    return box_;
}

bool BoxCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return sphere->Primitive().Intersects(box_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(box_);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return box->Primitive().Intersects(box_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return frustum->Primitive().Intersects(box_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return box_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle->Primitive().Intersects(box_);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool BoxCollision::Intersects(const math::Ray& ray, float& dist) const {
    return box_.Intersects(ray.position, ray.direction, dist);
}

FrustumCollision::FrustumCollision(const PrimitiveType& primitive) : frustum_{primitive} {}

auto FrustumCollision::Primitive() const -> const PrimitiveType& {
    return frustum_;
}

auto FrustumCollision::Primitive() -> PrimitiveType& {
    return frustum_;
}

bool FrustumCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return sphere->Primitive().Intersects(frustum_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(frustum_);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return box->Primitive().Intersects(frustum_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return frustum->Primitive().Intersects(frustum_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return frustum_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle->Primitive().Intersects(frustum_);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(frustum_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool FrustumCollision::Intersects(const math::Ray& ray, float& dist) const {
    return frustum_.Intersects(ray.position, ray.direction, dist);
}

PlaneCollision::PlaneCollision(const PrimitiveType& primitive) : plane_{primitive} {}

auto PlaneCollision::Primitive() const -> const PrimitiveType& {
    return plane_;
}

auto PlaneCollision::Primitive() -> PrimitiveType& {
    return plane_;
}

bool PlaneCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return sphere->Primitive().Intersects(plane_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(plane_);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return box->Primitive().Intersects(plane_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return frustum->Primitive().Intersects(plane_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return math::Intersects(plane->Primitive(), plane_);
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle->Primitive().Intersects(plane_);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(plane_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool PlaneCollision::Intersects(const math::Ray& ray, float& dist) const {
    return ray.Intersects(plane_, dist);
}

TriangleCollision::TriangleCollision(const PrimitiveType& primitive) : triangle_{primitive} {}

auto TriangleCollision::Primitive() const -> const PrimitiveType& {
    return triangle_;
}

auto TriangleCollision::Primitive() -> PrimitiveType& {
    return triangle_;
}

bool TriangleCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        return triangle_.Intersects(sphere->Primitive());
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        return triangle_.Intersects(axis_aligned_box->Primitive());
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        return triangle_.Intersects(box->Primitive());
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        return triangle_.Intersects(frustum->Primitive());
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        return triangle_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        return triangle_.Intersects(triangle->Primitive());
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(triangle_); };
        return std::ranges::any_of(mesh->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

bool TriangleCollision::Intersects(const math::Ray& ray, float& dist) const {
    return triangle_.Intersects(ray, dist);
}

MeshCollision::MeshCollision(const VertexCollection& vertices, const IndexCollection& indices)
    : vertices_{vertices}, indices_{indices} {}

MeshCollision::MeshCollision(VertexCollection&& vertices, IndexCollection&& indices)
    : vertices_{std::move(vertices)}, indices_{std::move(indices)} {}

auto MeshCollision::Vertices() const -> const VertexCollection& {
    return vertices_;
}

auto MeshCollision::Vertices() -> VertexCollection& {
    return vertices_;
}

auto MeshCollision::Indices() const -> const IndexCollection& {
    return indices_;
}

auto MeshCollision::Indices() -> IndexCollection& {
    return indices_;
}

bool MeshCollision::Intersects(const Collision& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(sphere->Primitive()); };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) {
            return triangle.Intersects(axis_aligned_box->Primitive());
        };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto box = dynamic_cast<const BoxCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box->Primitive()); };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(frustum->Primitive()); };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto plane = dynamic_cast<const PlaneCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(plane->Primitive()); };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto triangle = dynamic_cast<const TriangleCollision*>(&other)) {
        auto intersects = [=](const math::Triangle& other_triangle) {
            return other_triangle.Intersects(triangle->Primitive());
        };
        return std::ranges::any_of(Triangles(), intersects);
    }
    if (const auto mesh = dynamic_cast<const MeshCollision*>(&other)) {
        for (const math::Triangle& triangle : Triangles()) {
            // ReSharper disable once CppTooWideScopeInitStatement
            auto intersects = [=](const math::Triangle& other_triangle) { return triangle.Intersects(other_triangle); };
            if (std::ranges::any_of(mesh->Triangles(), intersects)) {
                return true;
            }
        }
        return false;
    }
    return other.Intersects(*this);
}

bool MeshCollision::Intersects(const math::Ray& ray, float& dist) const {
    auto intersects = [&](const math::Triangle& triangle) { return triangle.Intersects(ray, dist); };
    return std::ranges::any_of(Triangles(), intersects);
}

}  // namespace borov_engine
