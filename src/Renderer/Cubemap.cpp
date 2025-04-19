#include "Cubemap.h"
#include "Core/Log.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Athena
{
    Cubemap::Cubemap(const std::vector<std::string>& faces)
        : _textureHandle(0)
    {
        glGenTextures(1, &_textureHandle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureHandle);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // load and generate the texture
        stbi_set_flip_vertically_on_load(false);

        int width, height, nrChannels;
        for (size_t i = 0; i < faces.size(); ++i)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) 
            {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                    GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else 
            {
                LOG_ERROR("Failed to load cubemap texture '{}' (idx={})", faces[i], i);
                stbi_image_free(data);
            }
        }
    }

    Cubemap::~Cubemap()
    {
        glDeleteTextures(1, &_textureHandle);
    }

    void Cubemap::Bind(uint32_t index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureHandle);
    }
}
