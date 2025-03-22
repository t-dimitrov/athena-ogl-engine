#include "Renderer.h"
#include "Core/Log.h"

#include <glad/glad.h>
#include "imgui.h"
#include <glm/ext/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Athena
{
    Renderer::Renderer()
        : _clearColor(0.1f, 0.1f, 0.1f, 1.0f)
    {
        _materialLibrary = Ref<MaterialLibrary>::Create();
        _textureLibrary = Ref<TextureLibrary>::Create();
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Init()
    {
        _materialLibrary->Init();
        _textureLibrary->Init({
            { "ice", "assets/Textures/ice.jpg" }
        });

        //_model = Ref<Model>::Create("assets/Models/suzanne/suzanne.gltf");
        _model = Ref<Model>::Create("assets/Models/survival_guitar_backpack/scene.gltf");
        //_model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/ABeautifulGame/glTF/ABeautifulGame.gltf");
        //_model = Ref<Model>::Create("assets/Models/main1_sponza/NewSponza_Main_glTF_003.gltf");
        //_model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/NormalTangentTest/glTF/NormalTangentTest.gltf");

        _shader = Ref<Shader>::Create("assets/Shaders/Simple.vert.glsl", "assets/Shaders/Simple.frag.glsl");
    }

    void Renderer::Shutdown()
    {
        _textureLibrary->Release();

        _materialLibrary->Release();
    }

    void Renderer::BeginFrame(const Ref<EditorCamera>& camera)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
        _shader->Bind();
        _shader->SetUniformMat4("u_projection", camera->GetProjection());
        _shader->SetUniformMat4("u_view", camera->GetView());

        _shader->SetUniformFloat3("light.direction", _lightDirection);
        _shader->SetUniformFloat("light.ambientStrength", _lightAmbientStrength);
        _shader->SetUniformFloat3("light.color", _lightColor);
    }

    void Renderer::EndFrame()
    {
        glm::mat4 modelRotationMatrix =
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 modelTransform = 
            glm::translate(glm::mat4(1.0f), _modelPosition) *
            modelRotationMatrix *
            glm::scale(glm::mat4(1.0f), _modelScale);

        _model->Draw(_shader, modelTransform);
    }

    void Renderer::OnImGuiRender()
    {
        ImGui::Begin("Renderer");
        ImGui::ColorEdit4("Clear Color", &_clearColor.r);

        ImGui::Separator();

        ImGui::Text("Light");
        ImGui::DragFloat3("Direction", &_lightDirection.x);
        ImGui::DragFloat("Ambient Strength", &_lightAmbientStrength, 0.05f, 0.0f, 1.0f);
        ImGui::ColorEdit3("Color", &_lightColor.r);

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
        {
            const auto& materials = _materialLibrary->GetAllMaterials();
            for (const auto& kvp : materials)
            {
                if (ImGui::TreeNode(kvp.first.c_str()))
                {
                    Ref<Material> material = kvp.second;
                    if (material->albedoTexture)
                    {
                        ImGui::Text("Albedo Texture");
                        ImGui::Image(material->albedoTexture->GetTextureId(), ImVec2{ 100.0f, 100.0f }, ImVec2{0, 1}, ImVec2{1, 0});
                    }
                    if (material->normalTexture)
                    {
                        ImGui::Text("Normal Texture");
                        ImGui::Image(material->normalTexture->GetTextureId(), ImVec2{ 100.0f, 100.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    }

                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::Begin("Model");
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", &_modelPosition.x, 0.1f);
        ImGui::DragFloat3("Rotation", &_modelRotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &_modelScale.x, 0.1f);
        ImGui::Separator();
        _model->OnImGuiRender();
        ImGui::End();
    }
}
