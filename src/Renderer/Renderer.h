#pragma once
#include "Core/Ref.h"
#include "Shader.h"

namespace Athena
{
    class Renderer : public RefCounted
    {
    public:
        Renderer();
        ~Renderer();

        void Init();
        void Shutdown();

        void Render();

    private:
        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ibo;

        Ref<Shader> _shader;
    };
}
