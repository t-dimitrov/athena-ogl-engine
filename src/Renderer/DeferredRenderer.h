#pragma once
#include "Core/Ref.h"
#include "Core/Event/Event.h"
#include "Camera/EditorCamera.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Scene/Model.h"

#include <glm/glm.hpp>

namespace Athena
{
    class DeferredRenderer : public RefCounted
    {
    public:
        DeferredRenderer();
        ~DeferredRenderer();

        void Init();
        void Shutdown();

        void BeginFrame(const Ref<EditorCamera>& camera);
        void EndFrame();

        void OnEvent(Event& e);
        void OnImGuiRender();

    private:
        glm::vec4 _clearColor;

        Ref<Shader> _shader;
        Ref<UniformBuffer> _uniformBuffer;
        
        Ref<Framebuffer> _gBuffer;
        Ref<Model> _model;

        // Screen
        Ref<Shader> _screenShader;
        Ref<VertexArray> _screenVAO;
        Ref<VertexBuffer> _quadVertexBuffer;
        Ref<IndexBuffer> _quadIndexBuffer;
        //
    };
}
