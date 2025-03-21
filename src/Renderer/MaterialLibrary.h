#pragma once
#include "Core/Ref.h"
#include "Material.h"

#include <unordered_map>
#include <string>

namespace Athena
{
    class MaterialLibrary : public RefCounted
    {
    public:
        using MaterialMap = std::unordered_map<std::string, Ref<Material>>;

        MaterialLibrary();
        ~MaterialLibrary();

        void Init();
        void Release();

        Ref<Material> Add(const std::string& key, const Ref<Material>& material);
        Ref<Material> Get(const std::string& key) const;
        bool Has(const std::string& key) const;

        inline const MaterialMap& GetAllMaterials() const { return _materials; }
        inline uint32_t Count() const { return static_cast<uint32_t>(_materials.size()); }

    public:
        static MaterialLibrary& Instance() { return *s_instance; }

    private:
        static MaterialLibrary* s_instance;

        MaterialMap _materials;
    };
}
