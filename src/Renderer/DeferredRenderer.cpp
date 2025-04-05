#include "DeferredRenderer.h"
#include "Core/Event/WindowEvents.h"

#include <glad/glad.h>
#include <imgui.h>

namespace Athena
{
    DeferredRenderer::DeferredRenderer()
        : _clearColor(0.1f, 0.1f, 0.1f, 1.0f)
    {
    }

    DeferredRenderer::~DeferredRenderer()
    {
    }

    void DeferredRenderer::Init()
    {
        FramebufferDescriptor gBufferDesc{};
        gBufferDesc.debugName = "G Buffer";
        gBufferDesc.attachments = {
            { FramebufferTextureFormat::RGBA16 },   // Position color buffer
            { FramebufferTextureFormat::RGBA16 },   // Normal color buffer
            { FramebufferTextureFormat::Color },    // Color + Specular color buffer
            { FramebufferTextureFormat::Depth },    // Depth buffer
        };
        _gBuffer = Ref<Framebuffer>::Create(gBufferDesc);

        _shader = Ref<Shader>::Create("assets/Shaders/Deferred.vert.glsl", "assets/Shaders/Deferred.frag.glsl");

        _uniformBuffer = Ref<UniformBuffer>::Create(static_cast<uint32_t>(sizeof(glm::mat4) + sizeof(glm::vec3)), 0);

        // Screen
        {
            _screenShader = Ref<Shader>::Create("assets/Shaders/Screen.vert.glsl", "assets/Shaders/Screen.frag.glsl");
            _screenVAO = Ref<VertexArray>::Create();
            _screenVAO->Bind();
            {
                float quadVertices[16] = {
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
        }

        _model = Ref<Model>::Create("assets/Models/survival_guitar_backpack/scene.gltf");
        //_model = Ref<Model>::Create("assets/Models/glTF-Sample-Models/Sponza/glTF/Sponza.gltf");
    }

    void DeferredRenderer::Shutdown()
    {
    }

    void DeferredRenderer::BeginFrame(const Ref<EditorCamera>& camera)
    {
        // Setup camera UBO
        glm::mat4 viewProjMatrix = camera->GetProjection() * camera->GetView();
        _uniformBuffer->SetData(&viewProjMatrix, sizeof(glm::mat4), 0);
        _uniformBuffer->SetData(&camera->GetPosition(), sizeof(glm::vec3), sizeof(glm::mat4));

        // Todo: Use the position from the uniform buffer
        _shader->Bind();
        _shader->SetUniformFloat3("viewPos", camera->GetPosition());
    }

    void DeferredRenderer::EndFrame()
    {
        // Geometry pass
        {
            _gBuffer->Bind();
            glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, gPosition);
            //glActiveTexture(GL_TEXTURE1);
            //glBindTexture(GL_TEXTURE_2D, gNormal);
            //glActiveTexture(GL_TEXTURE2);
            //glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

            //// also send light relevant uniforms
            //shaderLightingPass.use();
            //SendAllLightUniformsToShader(shaderLightingPass);
            //shaderLightingPass.setVec3("viewPos", camera.Position);
            //RenderQuad();
            _model->Draw(_shader, glm::mat4(1.0f));
            _gBuffer->Unbind();
        }

        // Lighting pass
        {

        }

        // Screen pass
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            _screenShader->Bind();
            _screenVAO->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _gBuffer->GetColorAttachmentId(0));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void DeferredRenderer::OnEvent(Event& e)
    {
        if (e.GetEventType() == EventType::WindowResize)
        {
            WindowResizeEvent& wre = (WindowResizeEvent&)e;
            _gBuffer->Resize(wre.GetWidth(), wre.GetHeight());
        }
    }

    void DeferredRenderer::OnImGuiRender()
    {
        ImGui::Begin("Renderer");
        ImGui::ColorEdit4("Clear Color", &_clearColor.r);

        ImGui::Separator();

        ImGui::Text("Light");

        ImGui::Begin("Position color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(0), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::Begin("Normal color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(1), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::Begin("Albedo color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(2), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();

        ImGui::End();
    }
}
