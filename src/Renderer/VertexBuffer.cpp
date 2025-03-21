#include "VertexBuffer.h"
#include <glad/glad.h>

namespace Athena
{
    VertexBuffer::VertexBuffer(void* data, uint32_t byteSize)
    {
        glGenBuffers(1, &_bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(const void* data, uint32_t byteSize)
    {
        glGenBuffers(1, &_bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &_bufferId);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    }

    void VertexBuffer::SetLayout(const VertexBufferLayout& layout) const
    {
        ATH_ASSERT(layout.GetElements().size() != 0, "Vertex buffer layout is empty!");

        glBindBuffer(GL_ARRAY_BUFFER, _bufferId);

        int attribIndex = 0;
        uint32_t stride = layout.GetStride();
        for (auto& element : layout)
        {
            glEnableVertexAttribArray(attribIndex);
            glVertexAttribPointer(attribIndex,
                element.GetComponentCount(),
                element.GetShaderDataType(),
                element.normalized,
                stride,
                (const void*)(intptr_t)element.offset);

            attribIndex++;
        }
    }
}
