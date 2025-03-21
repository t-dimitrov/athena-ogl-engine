#pragma once
#include "Core/Ref.h"
#include "Texture2D.h"

namespace Athena
{
    struct Material : public RefCounted
    {
        Ref<Texture2D> albedoTexture = nullptr;

        Ref<Texture2D> normalTexture = nullptr;
        
        Ref<Texture2D> metallicTexture = nullptr;
        
        Ref<Texture2D> roughnessTexture = nullptr;
        
        Ref<Texture2D> ambientOcclusionTexture = nullptr;
    };
}
