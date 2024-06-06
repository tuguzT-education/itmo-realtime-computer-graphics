#include "borov_engine/math.hpp"

#include <array>
#include <numeric>

namespace borov_engine::math {

void Normalize(const Vector3 &vector, Vector3 &result) {
    vector.Normalize(result);
}

Vector3 Normalize(const Vector3 &vector) {
    Vector3 result;
    Normalize(vector, result);
    return result;
}

void RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by,
                  Vector3 &result) {
    result = Vector3::Transform(position - rotation_point, rotate_by) + rotation_point;
}

Vector3 RotateAround(const Vector3 &position, const Vector3 &rotation_point, const Quaternion &rotate_by) {
    Vector3 result;
    RotateAround(position, rotation_point, rotate_by, result);
    return result;
}

bool Intersects(const Plane lhs, const Plane rhs) {
    DirectX::XMVECTOR line_point_1_xm, line_point_2_xm;
    XMPlaneIntersectPlane(&line_point_1_xm, &line_point_2_xm, lhs, rhs);

    const Vector3 line_point_1{line_point_1_xm}, line_point_2{line_point_2_xm};
    auto all_nan = [](const Vector3 &vector) {
        auto &[x, y, z] = vector;
        return std::isnan(x) && std::isnan(y) && std::isnan(z);
    };
    const bool parallel = all_nan(line_point_1) && all_nan(line_point_2);
    return !parallel;
}

std::array<Vector3, Box::CORNER_COUNT> Corners(const Box &box) {
    std::array<Vector3, Box::CORNER_COUNT> corners;
    box.GetCorners(corners.data());
    return corners;
}

std::array<Vector3, AxisAlignedBox::CORNER_COUNT> Corners(const AxisAlignedBox &box) {
    std::array<Vector3, AxisAlignedBox::CORNER_COUNT> corners;
    box.GetCorners(corners.data());
    return corners;
}

std::array<Vector3, Frustum::CORNER_COUNT> Corners(const Frustum &frustum) {
    std::array<Vector3, Frustum::CORNER_COUNT> corners;
    frustum.GetCorners(corners.data());
    return corners;
}

Vector3 FrustumCenter(const Frustum &frustum) {
    const auto corners = Corners(frustum);
    return std::accumulate(std::begin(corners), std::end(corners), Vector3::Zero) / static_cast<float>(corners.size());
}

Vector3 Triangle::Tangent() const {
    return Normalize(point1 - point0);
}

Vector3 Triangle::Normal() const {
    const Vector3 cross = (point1 - point0).Cross(point2 - point1);
    return Normalize(cross);
}

Vector3 Triangle::Binormal() const {
    const Vector3 tangent = Tangent();
    const Vector3 normal = Normal();
    return tangent.Cross(normal);
}

Plane Triangle::Plane() const {
    return math::Plane{point0, point1, point2};
}

bool Triangle::Intersects(const AxisAlignedBox &axis_aligned_box) const {
    return axis_aligned_box.Intersects(point0, point1, point2);
}

bool Triangle::Intersects(const Box &box) const {
    return box.Intersects(point0, point1, point2);
}

bool Triangle::Intersects(const Sphere sphere) const {
    return sphere.Intersects(point0, point1, point2);
}

bool Triangle::Intersects(const Frustum &frustum) const {
    return frustum.Intersects(point0, point1, point2);
}

bool Triangle::Intersects(const Ray &ray, float &dist) const {
    return ray.Intersects(point0, point1, point2, dist);
}

bool Triangle::Intersects(const Triangle &other) const {
    return DirectX::TriangleTests::Intersects(point0, point1, point2, other.point0, other.point1, other.point2);
}

PlaneIntersectionType Triangle::Intersects(const math::Plane plane) const {
    return DirectX::TriangleTests::Intersects(point0, point1, point2, plane);
}

ContainmentType Triangle::ContainedBy(const AxisAlignedBox &axis_aligned_box) const {
    return axis_aligned_box.Contains(point0, point1, point2);
}

ContainmentType Triangle::ContainedBy(const Box &box) const {
    return box.Contains(point0, point1, point2);
}

ContainmentType Triangle::ContainedBy(const Sphere sphere) const {
    return sphere.Contains(point0, point1, point2);
}

ContainmentType Triangle::ContainedBy(const Frustum &frustum) const {
    return frustum.Contains(point0, point1, point2);
}

ContainmentType Triangle::ContainedBy(const math::Plane plane0, const math::Plane plane1, const math::Plane plane2,
                                      const math::Plane plane3, const math::Plane plane4,
                                      const math::Plane plane5) const {
    return DirectX::TriangleTests::ContainedBy(point0, point1, point2, plane0, plane1, plane2, plane3, plane4, plane5);
}

}  // namespace borov_engine::math
