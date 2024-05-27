#pragma once

#ifndef BOROV_ENGINE_MESH_COMPONENT_HPP_INCLUDED
#define BOROV_ENGINE_MESH_COMPONENT_HPP_INCLUDED

#include "triangle_component.hpp"

namespace borov_engine {

template <std::derived_from<TriangleComponent> ChildMesh = TriangleComponent>
class MeshComponent : public SceneComponent {
  public:
    using Vertex = typename ChildMesh::Vertex;
    using Index = typename ChildMesh::Index;

    struct Initializer : SceneComponent::Initializer {
        std::filesystem::path mesh_path;
    };

    explicit MeshComponent(class Game &game, const Initializer &initializer);

    void LoadMesh(const std::filesystem::path &mesh_path);
};

}  // namespace borov_engine

#include "mesh_component.inl"

#endif  // BOROV_ENGINE_MESH_COMPONENT_HPP_INCLUDED
