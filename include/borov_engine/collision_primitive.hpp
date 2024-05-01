#pragma once

#ifndef BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED
#define BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED

#include "math.hpp"

namespace borov_engine {

class CollisionPrimitive {
  public:
    virtual ~CollisionPrimitive();

    [[nodiscard]] virtual bool Intersects(const CollisionPrimitive &other) const = 0;
    [[nodiscard]] virtual bool Intersects(const math::Ray &ray, float &dist) const = 0;
};

class SphereCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::Sphere;

    explicit SphereCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType sphere_;
};

class AxisAlignedBoxCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::AxisAlignedBox;

    explicit AxisAlignedBoxCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType box_;
};

class BoxCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::Box;

    explicit BoxCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType box_;
};

class FrustumCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::Frustum;

    explicit FrustumCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType frustum_;
};

class PlaneCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::Plane;

    explicit PlaneCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType plane_;
};

class TriangleCollisionPrimitive : public CollisionPrimitive {
  public:
    using PrimitiveType = math::Triangle;

    explicit TriangleCollisionPrimitive(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType triangle_;
};

class CustomCollisionPrimitive : public CollisionPrimitive {
  public:
    using Vertex = math::Vector3;
    using VertexCollection = std::vector<Vertex>;

    using Index = std::uint32_t;
    using IndexCollection = std::vector<Index>;

    explicit CustomCollisionPrimitive(const VertexCollection &vertices, const IndexCollection &indices);
    explicit CustomCollisionPrimitive(VertexCollection &&vertices, IndexCollection &&indices);

    [[nodiscard]] const VertexCollection &Vertices() const;
    [[nodiscard]] VertexCollection &Vertices();

    [[nodiscard]] const IndexCollection &Indices() const;
    [[nodiscard]] IndexCollection &Indices();

    [[nodiscard]] std::vector<math::Triangle> Triangles() const;

    [[nodiscard]] bool Intersects(const CollisionPrimitive &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    VertexCollection vertices_;
    IndexCollection indices_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED
