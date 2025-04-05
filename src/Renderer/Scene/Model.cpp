#include "Model.h"
#include "Core/Log.h"

#include "Renderer/MaterialLibrary.h"
#include "Renderer/TextureLibrary.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <filesystem>

namespace Athena
{
    Model::Model(const std::string& filename)
    {
        LoadGLTF(filename);
    }

    Model::~Model()
    {
    }

    void Model::Draw(const Ref<Shader>& shader, const glm::mat4& transform) const
    {
        MaterialLibrary& materialLib = MaterialLibrary::Instance();
        for (int i = 0; i < _meshes.size(); ++i)
        {
            _meshes[i]->Draw(shader, materialLib.Get(_meshMaterialNames[i]), transform * _meshTransforms[i]);
        }
    }

    void Model::OnImGuiRender()
    {
        if (ImGui::TreeNode("Nodes"))
        {
            for (int i = 0; i < _nodeNames.size(); ++i)
            {
                ImGui::Text(_nodeNames[i].c_str());
            }

            ImGui::TreePop();
        }
    }

    void Model::LoadGLTF(const std::string& filename)
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

        if (!warn.empty())
        {
            LOG_WARN("Model '{}': {}", filename, warn);
        }

        if (!err.empty())
        {
            LOG_ERROR("Model '{}': {}", filename, err);
        }

        if (!ret)
        {
            LOG_ERROR("Failed to parse glTF '{}'", filename);
        }

        auto path = std::filesystem::path(filename);
        _directory = path.parent_path().string() + "/";

        const tinygltf::Scene& scene = model.defaultScene > -1
            ? model.scenes[model.defaultScene]
            : model.scenes[0];

        LOG_INFO("Loading Scene {} (nodes {})", scene.name, scene.nodes.size());

        for (int j = 0; j < scene.nodes.size(); ++j)
        {
            TraverseNode(model, scene.nodes[j]);
        }
    }

    void Model::TraverseNode(const tinygltf::Model& model, uint32_t nodeIndex, const glm::mat4& transform)
    {
        const tinygltf::Node& node = model.nodes[nodeIndex];
        glm::mat4 nodeTransform = transform * GetNodeTransform(node);

        LOG_DEBUG("Traversing Node {} (index {})", node.name, nodeIndex);

        if (node.mesh > -1)
        {
            LoadMesh(model, node.mesh, nodeTransform);
            _nodeNames.push_back(node.name);

        }

        for (int childIndex : node.children)
        {
            TraverseNode(model, childIndex, nodeTransform);
        }
    }

    void Model::LoadMesh(const tinygltf::Model& model, uint32_t meshIndex, const glm::mat4& transform)
    {
        const tinygltf::Mesh& mesh = model.meshes[meshIndex];

        for (int primitiveIndex = 0; primitiveIndex < mesh.primitives.size(); ++primitiveIndex)
        {
            const tinygltf::Primitive& primitive = mesh.primitives[primitiveIndex];

            // Extract indices
            ATH_ASSERT(primitive.indices > -1, "Primitive indices for mesh {} are not valid!", mesh.name);
            std::vector<uint32_t> indices = ExtractIndices(model, primitive.indices);

            // Extract vertex data
            std::vector<float> positionData = ExtractBufferData(model, primitive.attributes.at("POSITION"));
            std::vector<float> normalData = ExtractBufferData(model, primitive.attributes.at("NORMAL"));

            std::vector<float> uvData;
            if (primitive.attributes.count("TEXCOORD_0"))
                uvData = ExtractBufferData(model, primitive.attributes.at("TEXCOORD_0"));
            
            std::vector<float> tangentData;
            if (primitive.attributes.count("TANGENT"))
                tangentData = ExtractBufferData(model, primitive.attributes.at("TANGENT"));

            std::vector<Mesh::Vertex> vertices(positionData.size() / 3); // 3 floats = vertex position
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                vertices[i].position = glm::vec3(
                    positionData[i * 3 + 0],
                    positionData[i * 3 + 1],
                    positionData[i * 3 + 2]
                );

                vertices[i].normal = glm::vec3(
                    normalData[i * 3 + 0],
                    normalData[i * 3 + 1],
                    normalData[i * 3 + 2]
                );

                if (tangentData.size() > 0)
                {
                    vertices[i].tangent = glm::vec4(
                        tangentData[i * 3 + 3],
                        tangentData[i * 3 + 0],
                        tangentData[i * 3 + 1],
                        tangentData[i * 3 + 2]
                    );
                }
                else
                {
                    vertices[i].tangent = glm::vec4(1.0f);
                }

                if (uvData.size() > 0)
                {
                    vertices[i].uv = glm::vec2(
                        uvData[i * 2 + 0],
                        -uvData[i * 2 + 1]
                    );
                }
                else
                {
                    vertices[i].uv = glm::vec2(0.0f, 0.0f);
                }
            }

            Ref<Material> meshMaterial;
            std::string materialName;
            if (primitive.material > -1)
            {
                const tinygltf::Material& material = model.materials[primitive.material];
                materialName = material.name + std::to_string(primitive.material);

                MaterialLibrary& materialLibrary = MaterialLibrary::Instance();
                if (materialLibrary.Has(materialName))
                {
                    meshMaterial = materialLibrary.Get(materialName);
                }
                else
                {
                    meshMaterial = Ref<Material>::Create();

                    int baseColorTextureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
                    if (baseColorTextureIndex > -1)
                    {
                        std::string textureName = materialName + "_albedoTexture";
                        meshMaterial->albedoTexture = LoadTexture(model, textureName, baseColorTextureIndex);
                    }

                    int normalTextureIndex = material.normalTexture.index;
                    if (normalTextureIndex > -1)
                    {
                        std::string textureName = materialName + "_normalTexture";
                        meshMaterial->normalTexture = LoadTexture(model, textureName, normalTextureIndex);
                    }

                    materialLibrary.Add(materialName, meshMaterial);
                }
            }

            LOG_DEBUG("Mesh {} vertex count: {}, index count: {}", mesh.name, vertices.size(), indices.size());
            _meshes.push_back(Ref<Mesh>::Create(vertices, indices));
            _meshTransforms.push_back(transform);
            _meshMaterialNames.push_back(materialName);
        }
    }

    Ref<Texture2D> Model::LoadTexture(const tinygltf::Model& model, const std::string& textureName, int textureIndex)
    {
        int textureSource = model.textures[textureIndex].source;
        int textureSamplerIndex = model.textures[textureIndex].sampler;

        const tinygltf::Image& image = model.images[textureSource];
        const tinygltf::Sampler& sampler = model.samplers[textureSamplerIndex];

        if (!TextureLibrary::Instance().Has(textureName))
        {
            Ref<Texture2D> texture = Ref<Texture2D>::Create(
                _directory + image.uri,
                sampler.magFilter,
                sampler.minFilter,
                sampler.wrapS,
                sampler.wrapT);
            return TextureLibrary::Instance().Add(textureName, texture);
        }

        return TextureLibrary::Instance().Get(textureName);
    }

    glm::mat4 Model::GetNodeTransform(const tinygltf::Node& node)
    {
        glm::mat4 matrix = glm::mat4(1.0f);

        if (node.matrix.size())
        {
            matrix[0][0] = static_cast<float>(node.matrix[0]);
            matrix[0][1] = static_cast<float>(node.matrix[1]);
            matrix[0][2] = static_cast<float>(node.matrix[2]);
            matrix[0][3] = static_cast<float>(node.matrix[3]);

            matrix[1][0] = static_cast<float>(node.matrix[4]);
            matrix[1][1] = static_cast<float>(node.matrix[5]);
            matrix[1][2] = static_cast<float>(node.matrix[6]);
            matrix[1][3] = static_cast<float>(node.matrix[7]);

            matrix[2][0] = static_cast<float>(node.matrix[8]);
            matrix[2][1] = static_cast<float>(node.matrix[9]);
            matrix[2][2] = static_cast<float>(node.matrix[10]);
            matrix[2][3] = static_cast<float>(node.matrix[11]);

            matrix[3][0] = static_cast<float>(node.matrix[12]);
            matrix[3][1] = static_cast<float>(node.matrix[13]);
            matrix[3][2] = static_cast<float>(node.matrix[14]);
            matrix[3][3] = static_cast<float>(node.matrix[15]);
        }
        else
        {
            glm::mat4 translationMatrix = glm::mat4(1.0f);
            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            glm::mat4 scaleMatrix = glm::mat4(1.0f);

            if (node.translation.size())
            {
                translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
                    static_cast<float>(node.translation[0]),
                    static_cast<float>(node.translation[1]),
                    static_cast<float>(node.translation[2])
                ));
            }

            if (node.rotation.size())
            {
                rotationMatrix = glm::toMat4(glm::quat(
                    static_cast<float>(node.rotation[3]),
                    static_cast<float>(node.rotation[0]),
                    static_cast<float>(node.rotation[1]),
                    static_cast<float>(node.rotation[2])
                ));
            }

            if (node.scale.size())
            {
                glm::scale(glm::mat4(1.0f), glm::vec3(
                    static_cast<float>(node.scale[0]),
                    static_cast<float>(node.scale[1]),
                    static_cast<float>(node.scale[2])
                ));
            }

            matrix = translationMatrix * rotationMatrix * scaleMatrix;
        }

        return matrix;
    }

    std::vector<uint32_t> Model::ExtractIndices(const tinygltf::Model& model, uint32_t indicesAccessorIndex) const
    {
        const tinygltf::Accessor& indexAccessor = model.accessors[indicesAccessorIndex];
        const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

        size_t offset = indexBufferView.byteOffset + indexAccessor.byteOffset;

        std::vector<uint32_t> indices(indexAccessor.count);

        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            const uint16_t* indexData = reinterpret_cast<const uint16_t*>(&indexBuffer.data[offset]);
            for (size_t i = 0; i < indices.size(); ++i)
            {
                indices[i] = static_cast<uint32_t>(indexData[i]);
            }
        }
        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            const uint32_t* indexData = reinterpret_cast<const uint32_t*>(&indexBuffer.data[offset]);
            for (size_t i = 0; i < indices.size(); ++i)
            {
                indices[i] = indexData[i];
            }
        }
        else
        {
            LOG_ERROR("Unknown index accessor component type for primitive: {}", indexAccessor.componentType);
        }

        return indices;
    }

    std::vector<float> Model::ExtractBufferData(const tinygltf::Model& model, uint32_t accessorIndex) const
    {
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        size_t numPerVertex = 0;
        if (accessor.type == TINYGLTF_TYPE_SCALAR) numPerVertex = 1;
        else if (accessor.type == TINYGLTF_TYPE_VEC2) numPerVertex = 2;
        else if (accessor.type == TINYGLTF_TYPE_VEC3) numPerVertex = 3;
        else if (accessor.type == TINYGLTF_TYPE_VEC4) numPerVertex = 4;
        else LOG_ERROR("accessor type {} is invalid.", accessor.type);

        size_t offset = bufferView.byteOffset + accessor.byteOffset;
        size_t byteLength = accessor.count * sizeof(float) * numPerVertex;

        std::vector<float> data(accessor.count * numPerVertex);
        memcpy_s(data.data(), byteLength, &buffer.data[offset], byteLength);

        return data;
    }
}
