#include "borov_engine/collision_primitive.hpp"

#undef min
#undef max

#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

namespace borov_engine {

CollisionPrimitive::~CollisionPrimitive() = default;

bool CollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    return false;
}

SphereCollisionPrimitive::SphereCollisionPrimitive(const PrimitiveType& primitive) : sphere_{primitive} {}

auto SphereCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return sphere_;
}

auto SphereCollisionPrimitive::Primitive() -> PrimitiveType& {
    return sphere_;
}

bool SphereCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return sphere->Primitive().Intersects(sphere_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(sphere_);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return box->Primitive().Intersects(sphere_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return frustum->Primitive().Intersects(sphere_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return sphere_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle->Primitive().Intersects(sphere_);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(sphere_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

AxisAlignedBoxCollisionPrimitive::AxisAlignedBoxCollisionPrimitive(const PrimitiveType& primitive) : box_{primitive} {}

auto AxisAlignedBoxCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return box_;
}

auto AxisAlignedBoxCollisionPrimitive::Primitive() -> PrimitiveType& {
    return box_;
}

bool AxisAlignedBoxCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return sphere->Primitive().Intersects(box_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(box_);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return box->Primitive().Intersects(box_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return frustum->Primitive().Intersects(box_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return box_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle->Primitive().Intersects(box_);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

BoxCollisionPrimitive::BoxCollisionPrimitive(const PrimitiveType& primitive) : box_{primitive} {}

auto BoxCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return box_;
}

auto BoxCollisionPrimitive::Primitive() -> PrimitiveType& {
    return box_;
}

bool BoxCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return sphere->Primitive().Intersects(box_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(box_);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return box->Primitive().Intersects(box_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return frustum->Primitive().Intersects(box_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return box_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle->Primitive().Intersects(box_);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

FrustumCollisionPrimitive::FrustumCollisionPrimitive(const PrimitiveType& primitive) : frustum_{primitive} {}

auto FrustumCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return frustum_;
}

auto FrustumCollisionPrimitive::Primitive() -> PrimitiveType& {
    return frustum_;
}

bool FrustumCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return sphere->Primitive().Intersects(frustum_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(frustum_);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return box->Primitive().Intersects(frustum_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return frustum->Primitive().Intersects(frustum_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return frustum_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle->Primitive().Intersects(frustum_);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(frustum_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

PlaneCollisionPrimitive::PlaneCollisionPrimitive(const PrimitiveType& primitive) : plane_{primitive} {}

auto PlaneCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return plane_;
}

auto PlaneCollisionPrimitive::Primitive() -> PrimitiveType& {
    return plane_;
}

bool PlaneCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return sphere->Primitive().Intersects(plane_);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return axis_aligned_box->Primitive().Intersects(plane_);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return box->Primitive().Intersects(plane_);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return frustum->Primitive().Intersects(plane_);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return math::Intersects(plane->Primitive(), plane_);
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle->Primitive().Intersects(plane_);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(plane_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

TriangleCollisionPrimitive::TriangleCollisionPrimitive(const PrimitiveType& primitive) : triangle_{primitive} {}

auto TriangleCollisionPrimitive::Primitive() const -> const PrimitiveType& {
    return triangle_;
}

auto TriangleCollisionPrimitive::Primitive() -> PrimitiveType& {
    return triangle_;
}

bool TriangleCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(sphere->Primitive());
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(axis_aligned_box->Primitive());
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(box->Primitive());
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(frustum->Primitive());
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(plane->Primitive());
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        return triangle_.Intersects(triangle->Primitive());
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(triangle_); };
        return ranges::any_of(custom->Triangles(), intersects);
    }
    return other.Intersects(*this);
}

CustomCollisionPrimitive::CustomCollisionPrimitive(const VertexCollection& vertices, const IndexCollection& indices)
    : vertices_{vertices}, indices_{indices} {}

CustomCollisionPrimitive::CustomCollisionPrimitive(VertexCollection&& vertices, IndexCollection&& indices)
    : vertices_{std::move(vertices)}, indices_{std::move(indices)} {}

auto CustomCollisionPrimitive::Vertices() const -> const VertexCollection& {
    return vertices_;
}

auto CustomCollisionPrimitive::Vertices() -> VertexCollection& {
    return vertices_;
}

auto CustomCollisionPrimitive::Indices() const -> const IndexCollection& {
    return indices_;
}

auto CustomCollisionPrimitive::Indices() -> IndexCollection& {
    return indices_;
}

std::vector<math::Triangle> CustomCollisionPrimitive::Triangles() const {
    using namespace ranges;

    auto chunks = indices_ | views::chunk(3) | views::filter([](auto chunk) { return size(chunk) == 3; });
    auto triangles = chunks | views::transform([=](auto indices) {
                         return math::Triangle{
                             .point0 = vertices_[indices[0]],
                             .point1 = vertices_[indices[1]],
                             .point2 = vertices_[indices[2]],
                         };
                     });
    return triangles | to<std::vector>;
}

bool CustomCollisionPrimitive::Intersects(const CollisionPrimitive& other) const {
    if (const auto sphere = dynamic_cast<const SphereCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(sphere->Primitive()); };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto axis_aligned_box = dynamic_cast<const AxisAlignedBoxCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) {
            return triangle.Intersects(axis_aligned_box->Primitive());
        };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto box = dynamic_cast<const BoxCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(box->Primitive()); };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto frustum = dynamic_cast<const FrustumCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(frustum->Primitive()); };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto plane = dynamic_cast<const PlaneCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& triangle) { return triangle.Intersects(plane->Primitive()); };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto triangle = dynamic_cast<const TriangleCollisionPrimitive*>(&other)) {
        auto intersects = [=](const math::Triangle& my_triangle) {
            return my_triangle.Intersects(triangle->Primitive());
        };
        return ranges::any_of(Triangles(), intersects);
    }
    if (const auto custom = dynamic_cast<const CustomCollisionPrimitive*>(&other)) {
        for (const math::Triangle& my_triangle : Triangles()) {
            // ReSharper disable once CppTooWideScopeInitStatement
            auto intersects = [=](const math::Triangle& triangle) { return my_triangle.Intersects(triangle); };
            if (ranges::any_of(custom->Triangles(), intersects)) {
                return true;
            }
        }
        return false;
    }
    return other.Intersects(*this);
}

}  // namespace borov_engine
