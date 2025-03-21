#include "MaterialLibrary.h"
#include "Core/Log.h"

namespace Athena
{
    MaterialLibrary* MaterialLibrary::s_instance = nullptr;

    MaterialLibrary::MaterialLibrary()
    {
        s_instance = this;
    }

    MaterialLibrary::~MaterialLibrary()
    {
        s_instance = nullptr;
    }

    void MaterialLibrary::Init()
    {
    }

    void MaterialLibrary::Release()
    {
        _materials.clear();
    }

    Ref<Material> MaterialLibrary::Add(const std::string& key, const Ref<Material>& material)
    {
        if (!Has(key))
        {
            _materials.insert({ key, material });
            return material;
        }

        return _materials.at(key);
    }

    Ref<Material> MaterialLibrary::Get(const std::string& key) const
    {
        if (Has(key))
            return _materials.at(key);

        LOG_WARN("Failed to retrieve Material '{}'. Does not exist.", key);
        return nullptr;
    }

    bool MaterialLibrary::Has(const std::string& key) const
    {
        auto it = _materials.find(key);
        return it != _materials.end();
    }
}
