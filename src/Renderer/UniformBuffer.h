#pragma once
#include "Core/Ref.h"

namespace Athena
{
    class UniformBuffer : public RefCounted
    {
    public:
        UniformBuffer(uint32_t byteSize, uint32_t binding);
        ~UniformBuffer();

        void SetData(const void* data, uint32_t size, uint32_t offset);

    private:
        uint32_t _bufferId;
        uint8_t* _data;
    };
}
