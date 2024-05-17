#pragma once

#ifndef BOROV_ENGINE_MESH_COMPONENT_INL_INCLUDED
#define BOROV_ENGINE_MESH_COMPONENT_INL_INCLUDED

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <range/v3/view/enumerate.hpp>

namespace borov_engine {

namespace detail {

inline Transform TransformFromNode(const aiNode &node) {
    aiVector3D position, scale;
    aiQuaternion rotation;
    node.mTransformation.Decompose(scale, rotation, position);

    return Transform{
        .position = math::Vector3{position.x, position.y, position.z},
        .rotation = math::Quaternion{rotation.x, rotation.y, rotation.z, rotation.w},
        .scale = math::Vector3{scale.x, scale.y, scale.z},
    };
}

template <std::derived_from<TriangleComponent> ChildMesh>
ChildMesh &ChildFromMesh(Game &game, const SceneComponent &parent, const aiScene &scene, const aiMesh &mesh,
                         const std::filesystem::path &mesh_path) {
    math::Color diffuse{math::colors::linear::White};
    std::filesystem::path diffuse_texture_path;
    if (const aiMaterial *material = scene.mNumMaterials > 0 ? scene.mMaterials[mesh.mMaterialIndex] : nullptr) {
        if (aiColor3D ai_diffuse{1.0f, 1.0f, 1.0f};
            material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_diffuse) == aiReturn_SUCCESS) {
            diffuse = math::Color{ai_diffuse.r, ai_diffuse.g, ai_diffuse.b};
        }
        if (aiString ai_texture_diffuse;
            material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), ai_texture_diffuse) == aiReturn_SUCCESS) {
            diffuse_texture_path = mesh_path;
            diffuse_texture_path.remove_filename();
            diffuse_texture_path += std::string_view{ai_texture_diffuse.data, ai_texture_diffuse.length};
        }
    }

    std::vector<TriangleComponent::Vertex> vertices;
    for (const std::span ai_vertices{mesh.mVertices, mesh.mNumVertices};
         const auto &[index, ai_position] : ranges::views::enumerate(ai_vertices)) {
        const auto [x, y, z] = ai_position;
        const math::Vector3 position{x, y, z};

        math::Vector3 normal = math::Vector3::Right;
        if (const aiVector3D *ai_normals = mesh.mNormals) {
            const auto [x, y, z] = ai_normals[index];
            normal = math::Vector3{x, y, z};
        }

        math::Color color = diffuse;
        if (const aiColor4D *colors = mesh.mColors[0]) {
            const auto [r, g, b, a] = colors[index];
            color *= math::Color{r, g, b, a};
        }

        math::Vector2 texture_coordinate;
        if (const aiVector3D *texture_coordinates = mesh.mTextureCoords[0]) {
            const auto [x, y, z] = texture_coordinates[index];
            texture_coordinate = math::Vector2{x, y};
        }

        vertices.emplace_back(position, normal, color, texture_coordinate);
    }

    std::vector<TriangleComponent::Index> indices;
    for (const std::span faces{mesh.mFaces, mesh.mNumFaces}; const aiFace &face : faces) {
        for (const std::span ai_indices{face.mIndices, face.mNumIndices};
             const std::uint32_t index : ai_indices | std::views::take(3)) {
            indices.emplace_back(index);
        }
    }

    typename ChildMesh::Initializer initializer;
    initializer.vertices = vertices;
    initializer.indices = indices;
    initializer.texture_path = diffuse_texture_path;
    initializer.parent = &parent;
    return game.AddComponent<ChildMesh>(initializer);
}

template <std::derived_from<TriangleComponent> ChildMesh>
void TraverseNode(Game &game, const SceneComponent &parent, const aiScene &scene, const aiNode &node,
                  const std::filesystem::path &mesh_path) {
    const SceneComponent::Initializer root_args{.transform = TransformFromNode(node), .parent = &parent};
    const auto &root = game.AddComponent<SceneComponent>(root_args);

    for (const std::size_t mesh_index : std::span{node.mMeshes, node.mNumMeshes}) {
        if (const aiMesh *mesh = scene.mMeshes[mesh_index]) {
            ChildFromMesh<ChildMesh>(game, root, scene, *mesh, mesh_path);
        }
    }

    for (const aiNode *child_node : std::span{node.mChildren, node.mNumChildren}) {
        TraverseNode<ChildMesh>(game, root, scene, *child_node, mesh_path);
    }
}

}  // namespace detail

template <std::derived_from<TriangleComponent> ChildMesh>
auto MeshComponent<ChildMesh>::Initializer::MeshPath(const std::filesystem::path &mesh_path) -> Initializer & {
    this->mesh_path = mesh_path;
    return *this;
}

template <std::derived_from<TriangleComponent> ChildMesh>
MeshComponent<ChildMesh>::MeshComponent(class Game &game, const Initializer &initializer)
    : SceneComponent(game, initializer) {
    LoadMesh(initializer.mesh_path);
}

template <std::derived_from<TriangleComponent> ChildMesh>
void MeshComponent<ChildMesh>::LoadMesh(const std::filesystem::path &mesh_path) {
    if (!mesh_path.has_filename()) {
        return;
    }

    Assimp::Importer importer;
    constexpr std::uint32_t flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals;
    const aiScene *scene = importer.ReadFile(mesh_path.generic_string(), flags);
    if (scene == nullptr) {
        const char *message = importer.GetErrorString();
        throw std::runtime_error{message};
    }

    if (const aiNode *node = scene->mRootNode) {
        detail::TraverseNode<ChildMesh>(Game(), *this, *scene, *node, mesh_path);
    }
}

}  // namespace borov_engine

#endif  // BOROV_ENGINE_MESH_COMPONENT_INL_INCLUDED
