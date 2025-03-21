#pragma once
#include "Core/Ref.h"
#include "Mesh.h"
#include "Renderer/Texture2D.h"

#include "tiny_gltf.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Athena
{
    class Model : public RefCounted
    {
    public:
        Model(const std::string& filename);
        ~Model();

        void Draw(const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f)) const;

        void OnImGuiRender();

    private:
        void LoadGLTF(const std::string& filename);
        void TraverseNode(const tinygltf::Model& model, uint32_t nodeIndex, const glm::mat4& transform = glm::mat4(1.0f));
        void LoadMesh(const tinygltf::Model& model, uint32_t meshIndex, const glm::mat4& transform);
        Ref<Texture2D> LoadTexture(const tinygltf::Model& model, const std::string& textureName, int textureIndex);

        glm::mat4 GetNodeTransform(const tinygltf::Node& node);

        std::vector<uint32_t> ExtractIndices(const tinygltf::Model& model, uint32_t indicesAccessorIndex) const;
        std::vector<float> ExtractBufferData(const tinygltf::Model& model, uint32_t accessorIndex) const;

    private:
        std::string _directory;
        std::vector<Ref<Mesh>> _meshes;
        std::vector<std::string> _meshMaterialNames;
        std::vector<glm::mat4> _meshTransforms;

        std::vector<std::string> _nodeNames;
    };
}
