#include "TextureLibrary.h"
#include "Core/Log.h"

namespace Athena
{
    TextureLibrary* TextureLibrary::s_instance = nullptr;

    TextureLibrary::TextureLibrary()
    {
        s_instance = this;
    }

    TextureLibrary::~TextureLibrary()
    {
        s_instance = nullptr;
    }

    void TextureLibrary::Init()
    {
    }

    void TextureLibrary::Init(const std::initializer_list<std::pair<std::string, std::string>>& filenames)
    {
        for (const auto& pair : filenames)
        {
            Add(pair.first, pair.second);
        }
    }

    void TextureLibrary::Release()
    {
        _textures.clear();
    }

    Ref<Texture2D> TextureLibrary::Add(const std::string& key, const std::string& filename)
    {
        if (!Has(key))
        {
            auto texture = Ref<Texture2D>::Create(filename);
            _textures.insert({ key, texture });
            return texture;
        }

        return _textures.at(key);
    }

    Ref<Texture2D> TextureLibrary::Add(const std::string& key, const Ref<Texture2D>& texture)
    {
        if (!Has(key))
        {
            _textures.insert({ key, texture });
            return texture;
        }

        return _textures.at(key);
    }

    Ref<Texture2D> TextureLibrary::Get(const std::string& key) const
    {
        if (Has(key))
            return _textures.at(key);
        
        LOG_WARN("Failed to retrieve Texture2D '{}'. Does not exist.", key);
        return nullptr;
    }

    bool TextureLibrary::Has(const std::string& key) const
    {
        auto it = _textures.find(key);
        return it != _textures.end();
    }
}
