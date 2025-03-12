#include "IndexBuffer.h"
#include <glad/glad.h>

namespace Athena
{
    IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count)
        : _count(count)
    {
        glGenBuffers(1, &_bufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &_bufferId);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
    }
}
