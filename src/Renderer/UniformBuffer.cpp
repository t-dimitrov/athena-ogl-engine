#include "UniformBuffer.h"

#include <glad/glad.h>

namespace Athena
{
    UniformBuffer::UniformBuffer(uint32_t byteSize, uint32_t binding)
    {
        _data = new uint8_t[byteSize];

        glGenBuffers(1, &_bufferId);
        glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
        glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, binding, _bufferId, 0, byteSize);
    }

    UniformBuffer::~UniformBuffer()
    {
        delete[] _data;
        glDeleteBuffers(1, &_bufferId);
    }

    void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        memcpy(_data, data, size);
        glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}
