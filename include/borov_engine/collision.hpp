#pragma once

#ifndef BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED
#define BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED

#include "math.hpp"

namespace borov_engine {

class Collision {
  public:
    virtual ~Collision();

    [[nodiscard]] virtual bool Intersects(const Collision &other) const = 0;
    [[nodiscard]] virtual bool Intersects(const math::Ray &ray, float &dist) const = 0;
};

class SphereCollision : public Collision {
  public:
    using PrimitiveType = math::Sphere;

    explicit SphereCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType sphere_;
};

class AxisAlignedBoxCollision : public Collision {
  public:
    using PrimitiveType = math::AxisAlignedBox;

    explicit AxisAlignedBoxCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType box_;
};

class BoxCollision : public Collision {
  public:
    using PrimitiveType = math::Box;

    explicit BoxCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType box_;
};

class FrustumCollision : public Collision {
  public:
    using PrimitiveType = math::Frustum;

    explicit FrustumCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType frustum_;
};

class PlaneCollision : public Collision {
  public:
    using PrimitiveType = math::Plane;

    explicit PlaneCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType plane_;
};

class TriangleCollision : public Collision {
  public:
    using PrimitiveType = math::Triangle;

    explicit TriangleCollision(const PrimitiveType &primitive);

    [[nodiscard]] const PrimitiveType &Primitive() const;
    [[nodiscard]] PrimitiveType &Primitive();

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    PrimitiveType triangle_;
};

template <typename View>
concept TrianglesRange = std::ranges::range<View> && std::same_as<std::ranges::range_value_t<View>, math::Triangle>;

class MeshCollision : public Collision {
  public:
    using Vertex = math::Vector3;
    using VertexCollection = std::vector<Vertex>;

    using Index = std::uint32_t;
    using IndexCollection = std::vector<Index>;

    explicit MeshCollision(const VertexCollection &vertices, const IndexCollection &indices);
    explicit MeshCollision(VertexCollection &&vertices, IndexCollection &&indices);

    [[nodiscard]] const VertexCollection &Vertices() const;
    [[nodiscard]] VertexCollection &Vertices();

    [[nodiscard]] const IndexCollection &Indices() const;
    [[nodiscard]] IndexCollection &Indices();

    [[nodiscard]] TrianglesRange auto Triangles() const;

    [[nodiscard]] bool Intersects(const Collision &other) const override;
    [[nodiscard]] bool Intersects(const math::Ray &ray, float &dist) const override;

  private:
    VertexCollection vertices_;
    IndexCollection indices_;
};

}  // namespace borov_engine

#include "collision.inl"

#endif  // BOROV_ENGINE_COLLISION_PRIMITIVE_HPP_INCLUDED
