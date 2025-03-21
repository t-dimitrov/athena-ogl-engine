#pragma once
#include "Core/Ref.h"

namespace Athena
{
    class IndexBuffer : public RefCounted
    {
    public:
        IndexBuffer(uint32_t* data, uint32_t count);
        IndexBuffer(const uint32_t* data, uint32_t count);
        ~IndexBuffer();

        void Bind() const;

        inline uint32_t GetElementCount() const { return _count; }

    private:
        uint32_t _bufferId;
        uint32_t _count;
    };
}
