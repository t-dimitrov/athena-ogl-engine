#pragma once
#include "Core/Ref.h"
#include "Texture2D.h"

#include <unordered_map>
#include <string>

namespace Athena
{
    class TextureLibrary : public RefCounted
    {
    public:
        TextureLibrary();
        ~TextureLibrary();

        void Init(const std::initializer_list<std::pair<std::string, std::string>>& filenames);
        void Release();

        Ref<Texture2D> Add(const std::string& key, const std::string& filename);
        Ref<Texture2D> Add(const std::string& key, const Ref<Texture2D>& texture);
        Ref<Texture2D> Get(const std::string& key) const;
        bool Has(const std::string& key) const;
        uint32_t Count() const { return static_cast<uint32_t>(_textures.size()); }

        static TextureLibrary& Instance() { return *s_instance; }

    private:
        static TextureLibrary* s_instance;

        using TextureMap = std::unordered_map<std::string, Ref<Texture2D>>;
        TextureMap _textures;
    };
}
