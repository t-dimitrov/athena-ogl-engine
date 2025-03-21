#include "Texture2D.h"
#include "Core/Log.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

namespace Athena
{
    Texture2D::Texture2D(const std::string& filename)
    {
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        stbi_set_flip_vertically_on_load(true);

        int nrChannels;
        unsigned char* data = stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            LOG_ERROR("Failed to load texture ({})", filename);
        }
        stbi_image_free(data);
    }

    Texture2D::Texture2D(const std::string& filename, int32_t magFilter, int32_t minFilter, int32_t wrapS, int32_t wrapT)
    {
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        // load and generate the texture
        stbi_set_flip_vertically_on_load(true);

        int nrChannels;
        unsigned char* data = stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 0);
        if (data)
        {
            GLint format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            LOG_ERROR("Failed to load texture ({})", filename);
        }
        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &_textureId);
    }

    void Texture2D::Bind(uint32_t index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, _textureId);
    }
}
