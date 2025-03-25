#include "VertexArray.h"
#include <glad/glad.h>

namespace Athena
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &_vertexArrayId);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &_vertexArrayId);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(_vertexArrayId);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}
