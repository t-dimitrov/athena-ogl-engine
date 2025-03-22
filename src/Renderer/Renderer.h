#pragma once
#include "Core/Ref.h"
#include "Camera/EditorCamera.h"

#include "MaterialLibrary.h"
#include "TextureLibrary.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "Scene/Model.h"

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

        void OnImGuiRender();

    private:
        glm::vec4 _clearColor;

        Ref<MaterialLibrary> _materialLibrary;
        Ref<TextureLibrary> _textureLibrary;

        glm::vec3 _modelPosition = glm::vec3{ 0.0f };
        glm::vec3 _modelRotation = glm::vec3{ 0.0f };
        glm::vec3 _modelScale = glm::vec3{ 1.0f };
        Ref<Model> _model;
        Ref<Shader> _shader;

        // Light
        float _lightAmbientStrength = 0.1f;
        glm::vec3 _lightDirection = { -1.0f, -1.0f, -1.0f };
        glm::vec3 _lightColor = { 1.0f, 1.0f, 1.0f };
    };
}
