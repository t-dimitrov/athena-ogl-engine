#pragma once
#include "Core/Ref.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace Athena
{
    class Renderer : public RefCounted
    {
    public:
        Renderer();
        ~Renderer();

        void Init();
        void Shutdown();

        void Render();

    private:
        Ref<Shader> _shader;
        Ref<VertexArray> _vertexArray;
        Ref<VertexBuffer> _vertexBuffer;
        Ref<IndexBuffer> _indexBuffer;
    };
}
