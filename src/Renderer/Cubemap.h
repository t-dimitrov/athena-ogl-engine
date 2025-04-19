#pragma once
#include "Core/Ref.h"
#include <vector>
#include <string>

namespace Athena
{
    class Cubemap : public RefCounted
    {
    public:
        /* Cubemap faces should be in the following order:
            GL_TEXTURE_CUBE_MAP_POSITIVE_X  ->  Right
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X  ->  Left
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y  ->  Top
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y  ->  Bottom
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z  ->  Front
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  ->  Back
        */ 
        Cubemap(const std::vector<std::string>& faces);
        ~Cubemap();

        void Bind(uint32_t index = 0) const;

    private:
        uint32_t _textureHandle;
    };
}
