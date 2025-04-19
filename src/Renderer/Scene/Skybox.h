#pragma once
#include "Core/Ref.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Cubemap.h"
#include "Renderer/Shader.h"

namespace Athena
{
    class Skybox : public RefCounted
    {
    public:
        Skybox(const std::vector<std::string>& faces);
        ~Skybox();

        void Bind() const;

    private:
        Ref<Cubemap> _cubemap;
        Ref<VertexArray> _vertexArrayObject;
        Ref<VertexBuffer> _vertexBuffer;
        Ref<IndexBuffer> _indexBuffer;
        Ref<Shader> _shader;
    };
}
