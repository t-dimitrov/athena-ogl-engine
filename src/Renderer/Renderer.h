#pragma once
#include "Core/Ref.h"
#include "Camera/EditorCamera.h"

#include "MaterialLibrary.h"
#include "TextureLibrary.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "UniformBuffer.h"

#include "Scene/Model.h"

#include "Light/Light.h"

#include <glm/glm.hpp>

namespace Athena
{
    class Renderer : public RefCounted
    {
    public:
        Renderer();
        ~Renderer();

        void Init();
        void Shutdown();

        void BeginFrame(const Ref<EditorCamera>& camera);
        void EndFrame();

        void OnEvent(Event& e);
        void OnImGuiRender();

    private:
        glm::vec4 _clearColor;

        Ref<MaterialLibrary> _materialLibrary;
        Ref<TextureLibrary> _textureLibrary;

        Ref<Framebuffer> _framebuffer;
        Ref<Framebuffer> _fbShadowMap;

        Ref<Shader> _screenShader;
        Ref<Shader> _shadowMapShader;

        Ref<VertexArray> _screenVAO;
        Ref<VertexBuffer> _quadVertexBuffer;
        Ref<IndexBuffer> _quadIndexBuffer;
        Ref<UniformBuffer> _cameraUniformBuffer;

        glm::vec3 _modelPosition = glm::vec3{ 0.0f };
        glm::vec3 _modelRotation = glm::vec3{ 0.0f };
        glm::vec3 _modelScale = glm::vec3{ 1.0f };
        Ref<Model> _model;
        Ref<Model> _cubeModel;
        Ref<Shader> _shader;

        // Light
        DirectionalLight _directionalLight;
    };
}
