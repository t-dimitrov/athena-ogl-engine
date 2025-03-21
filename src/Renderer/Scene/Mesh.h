#pragma once
#include "Core/Ref.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Texture2D.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

#include <vector>
#include <glm/glm.hpp>

namespace Athena
{
    class Mesh : public RefCounted
    {
    public:
        struct Vertex 
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec4 tangent;
            glm::vec2 uv;
        };

    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();

        void Draw(const Ref<Shader>& shader, const Ref<Material>& material, const glm::mat4& transform) const;

    private:
        Ref<VertexArray> _vertexArray;
        Ref<VertexBuffer> _vertexBuffer;
        Ref<IndexBuffer> _indexBuffer;
    };
}
