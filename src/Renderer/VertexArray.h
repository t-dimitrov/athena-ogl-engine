#pragma once
#include "Core/Ref.h"

namespace Athena
{
    class VertexArray : public RefCounted
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;

    private:
        uint32_t _vertexArrayId;
    };
}
