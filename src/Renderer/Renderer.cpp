#include "Renderer.h"
#include "Core/Log.h"
#include "Core/Event/WindowEvents.h"

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

        FramebufferDescriptor framebufferDesc{};
        framebufferDesc.attachments = {
            { FramebufferTextureFormat::Color },
            { FramebufferTextureFormat::Depth }
        };
        framebufferDesc.debugName = "Main Framebuffer";
        _framebuffer = Ref<Framebuffer>::Create(framebufferDesc);

        framebufferDesc = {};
        framebufferDesc.attachments = {
            { FramebufferTextureFormat::Depth }
        };
        framebufferDesc.debugName = "Shadpw Map";
        _fbShadowMap = Ref<Framebuffer>::Create(framebufferDesc);

        _screenShader = Ref<Shader>::Create("assets/Shaders/Screen.vert.glsl", "assets/Shaders/Screen.frag.glsl");

        _shadowMapShader = Ref<Shader>::Create("assets/Shaders/ShadowMap.vert.glsl", "assets/Shaders/ShadowMap.frag.glsl");

        _screenVAO = Ref<VertexArray>::Create();
        _screenVAO->Bind();
        {
            float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f, 1.0f,
                -1.0f,  1.0f,  0.0f, 1.0f
            };
            _quadVertexBuffer = Ref<VertexBuffer>::Create(quadVertices, static_cast<uint32_t>(sizeof(quadVertices)));
            _quadVertexBuffer->SetLayout({
                { ShaderDataType::Float2, "a_Position" },
                { ShaderDataType::Float2, "a_TexCoord" },
            });

            uint32_t screenIndices[6] = {
                0, 1, 2,
                2, 3, 0
            };
            _quadIndexBuffer = Ref<IndexBuffer>::Create(screenIndices, 6);
        }
        _screenVAO->Unbind();

        _cameraUniformBuffer = Ref<UniformBuffer>::Create(static_cast<uint32_t>(sizeof(glm::mat4)), 0);

        //_model = Ref<Model>::Create("assets/Models/suzanne/suzanne.gltf");
        //_model = Ref<Model>::Create("assets/Models/survival_guitar_backpack/scene.gltf");
        //_model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/ABeautifulGame/glTF/ABeautifulGame.gltf");
        //_model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/Cube/glTF/Cube.gltf");
        _model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/Sponza/glTF/Sponza.gltf");
        _cubeModel = Ref<Model>::Create("assets/Models/glTF-Sample-Models/Cube/glTF/Cube.gltf");

        _shader = Ref<Shader>::Create("assets/Shaders/Simple.vert.glsl", "assets/Shaders/Simple.frag.glsl");
    }

    void Renderer::Shutdown()
    {
        _textureLibrary->Release();

        _materialLibrary->Release();
    }

    void Renderer::BeginFrame(const Ref<EditorCamera>& camera)
    {
        // Setup camera UBO
        glm::mat4 viewProjMatrix = camera->GetProjection() * camera->GetView();
        _cameraUniformBuffer->SetData(&viewProjMatrix, sizeof(glm::mat4), 0);

        // Setup model transform
        glm::mat4 modelRotationMatrix =
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 modelTransform =
            glm::translate(glm::mat4(1.0f), _modelPosition) *
            modelRotationMatrix *
            glm::scale(glm::mat4(1.0f), _modelScale);

        // Shadow pass
        {
            glEnable(GL_DEPTH_TEST);
            _fbShadowMap->Bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            _shadowMapShader->Bind();
            _shadowMapShader->SetUniformMat4("u_lightProjection", _directionalLight.orthogonalProjection * _directionalLight.GetView());

            _model->Draw(_shadowMapShader, modelTransform);
            _cubeModel->Draw(_shadowMapShader, glm::mat4(1.0f));

            _fbShadowMap->Unbind();
        }

        // Main Render pass
        {
            glEnable(GL_DEPTH_TEST);
            _framebuffer->Bind();
            glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            _shader->Bind();
            _shader->SetUniformFloat3("light.direction", _directionalLight.direction);
            _shader->SetUniformFloat("light.ambientStrength", _directionalLight.ambientStrength);
            _shader->SetUniformFloat3("light.color", _directionalLight.color);
            _shader->SetUniformMat4("light.projection", _directionalLight.orthogonalProjection * _directionalLight.GetView());

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, _fbShadowMap->GetDepthAttachmentId());
            _shader->SetUniformInt("u_shadowMap", 3);

            _model->Draw(_shader, modelTransform);
            _cubeModel->Draw(_shader, glm::mat4(1.0f));

            _framebuffer->Unbind();
        }
        
        // Screen pass

        // First pass
        /*_framebuffer->Bind();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shader->Bind();

        _model->Draw(_shader, modelTransform);*/
    }

    void Renderer::EndFrame()
    {
        // Screen pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        _screenShader->Bind();
        _screenVAO->Bind();
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, _fbShadowMap->GetDepthAttachmentId());
        glBindTexture(GL_TEXTURE_2D, _framebuffer->GetColorAttachmentId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Renderer::OnEvent(Event& e)
    {
        if (e.GetEventType() == EventType::WindowResize)
        {
            WindowResizeEvent& wre = (WindowResizeEvent&)e;
            _framebuffer->Resize(wre.GetWidth(), wre.GetHeight());
            _fbShadowMap->Resize(wre.GetWidth(), wre.GetHeight());
        }
    }

    void Renderer::OnImGuiRender()
    {
        ImGui::Begin("Renderer");
        ImGui::ColorEdit4("Clear Color", &_clearColor.r);

        ImGui::Separator();

        ImGui::Text("Light");
        ImGui::DragFloat("Ambient Strength", &_directionalLight.ambientStrength, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat3("Direction", &_directionalLight.direction.x);
        ImGui::ColorEdit3("Color", &_directionalLight.color.r);

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Framebuffer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
        {
            ImGui::Text("Color Attachment 0");
            ImGui::Image(_framebuffer->GetColorAttachmentId(0), ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::Text("Depth Attachment");
            ImGui::Image(_framebuffer->GetDepthAttachmentId(), ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::Text("Shadow Map");
            ImGui::Image(_fbShadowMap->GetDepthAttachmentId(), ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::TreePop();
        }
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
