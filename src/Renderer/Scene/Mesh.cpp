#include "Mesh.h"

#include <glad/glad.h>

namespace Athena
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        _vertexArray = Ref<VertexArray>::Create();
        _vertexArray->Bind();
        
        _vertexBuffer = Ref<VertexBuffer>::Create(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));
        _vertexBuffer->Bind();
        _vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float4, "a_Tangent" },
            { ShaderDataType::Float2, "a_UV" },
        });

        _indexBuffer = Ref<IndexBuffer>::Create(indices.data(), static_cast<uint32_t>(indices.size()));
        _indexBuffer->Bind();

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Draw(const Ref<Shader>& shader, const Ref<Material>& material, const glm::mat4& transform) const
    {
        shader->Bind();
        shader->SetUniformMat4("u_model", transform);
        
        shader->SetUniformInt("u_material.albedoTexture", 0);
        shader->SetUniformInt("u_material.normalTexture", 1);

        if (material->albedoTexture)
            material->albedoTexture->Bind(0);

        if (material->normalTexture)
            material->normalTexture->Bind(1);

        _vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, _indexBuffer->GetElementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
