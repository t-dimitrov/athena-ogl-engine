#pragma once
#include "Core/Ref.h"
#include <string>

namespace Athena
{
    class Texture2D : public RefCounted
    {
    public:
        Texture2D(const std::string& filename);
        Texture2D(const std::string& filename, int32_t magFilter, int32_t minFilter, int32_t wrapS, int32_t wrapT);
        ~Texture2D();

        void Bind(uint32_t index = 0) const;

        inline uint32_t GetTextureId() const { return _textureId; }

    private:
        uint32_t _textureId;
        int32_t _width, _height;
    };
}
