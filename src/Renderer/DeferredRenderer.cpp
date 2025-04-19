#include "DeferredRenderer.h"
#include "Core/Event/WindowEvents.h"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Athena
{
    std::string glGetDebugSourceName(GLenum source)
    {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:               return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "Window System";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY:       return "Third Party";
            case GL_DEBUG_SOURCE_APPLICATION:       return "Application";
            case GL_DEBUG_SOURCE_OTHER:             return "Other";
            default:                                return "Unknown OpenGL Debug Source";
        }
    }

    std::string glGetDebugTypeName(GLenum type)
    {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
            case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
            case GL_DEBUG_TYPE_MARKER:              return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
            case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
            case GL_DEBUG_TYPE_OTHER:               return "Other";
            default:                                return "Unkown OpenGL Debug Type";
        }
    }

    void APIENTRY glDebugOutput(GLenum source,
        GLenum type,
        unsigned int id,
        GLenum severity,
        GLsizei length,
        const char* message,
        const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::string sourceName = glGetDebugSourceName(source);
        std::string typeName = glGetDebugTypeName(type);

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
            {
                ATH_ASSERT(false, "OpenGL High:\n\tSource: {}\n\tType: {}\n\t({}): {}", sourceName, typeName, id, message);
                break;
            }

            case GL_DEBUG_SEVERITY_MEDIUM:
            {
                LOG_ERROR("OpenGL Medium:\n\tSource: {}\n\tType: {}\n\t({}): {}", sourceName, typeName, id, message);
                break;
            }
            case GL_DEBUG_SEVERITY_LOW:
            {
                LOG_WARN("OpenGL Low:\n\tSource: {}\n\tType: {}\n\t({}): {}", sourceName, typeName, id, message);
                break;
            }
            case GL_DEBUG_SEVERITY_NOTIFICATION:
            {
                //LOG_INFO("OpenGL Notification:\n\tSource: {}\n\tType: {}\n\t({}): {}", sourceName, typeName, id, message);
                break;
            }
        }
    }

    DeferredRenderer::DeferredRenderer()
        : _clearColor(0.1f, 0.1f, 0.1f, 1.0f)
    {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
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
            { FramebufferTextureFormat::RGBA8 },    // Color + Specular color buffer
            { FramebufferTextureFormat::DEPTH24 },  // Depth buffer
        };
        _gBuffer = Ref<Framebuffer>::Create(gBufferDesc);

        FramebufferDescriptor fbDesc{};
        fbDesc.debugName = "Screen Framebuffer";
        fbDesc.attachments = {
            { FramebufferTextureFormat::Color },
            { FramebufferTextureFormat::Depth }
        };
        _screenFramebuffer = Ref<Framebuffer>::Create(fbDesc);

        _gBufferShader = Ref<Shader>::Create("assets/Shaders/Deferred.vert.glsl", "assets/Shaders/Deferred.frag.glsl");
        _lightingShader = Ref<Shader>::Create("assets/Shaders/Lighting.vert.glsl", "assets/Shaders/Lighting.frag.glsl");

        _uniformBuffer = Ref<UniformBuffer>::Create(static_cast<uint32_t>(sizeof(glm::mat4) * 2 + sizeof(glm::vec3)), 0);

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

        _pointLights.push_back({ glm::vec3(75.0f, 50.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f) });

        // Skybox
        {
            _skybox = Ref<Skybox>::Create(std::vector<std::string>{
                "assets/Cubemaps/Storforsen2/posx.jpg",
                "assets/Cubemaps/Storforsen2/negx.jpg",
                "assets/Cubemaps/Storforsen2/posy.jpg",
                "assets/Cubemaps/Storforsen2/negy.jpg",
                "assets/Cubemaps/Storforsen2/posz.jpg",
                "assets/Cubemaps/Storforsen2/negz.jpg",
            });
        }
    }

    void DeferredRenderer::Shutdown()
    {
    }

    void DeferredRenderer::BeginFrame(const Ref<EditorCamera>& camera)
    {
        // Setup camera UBO
        _uniformBuffer->SetData(&camera->GetView(), sizeof(glm::mat4), 0);
        _uniformBuffer->SetData(&camera->GetProjection(), sizeof(glm::mat4), sizeof(glm::mat4));
        _uniformBuffer->SetData(&camera->GetPosition(), sizeof(glm::vec3), sizeof(glm::mat4) * 2);

        _viewMatrix = camera->GetView();
    }

    void DeferredRenderer::EndFrame()
    {
        // Setup model transform
        glm::mat4 modelRotationMatrix =
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 modelTransform =
            glm::translate(glm::mat4(1.0f), _modelPosition) *
            modelRotationMatrix *
            glm::scale(glm::mat4(1.0f), _modelScale);

        // Geometry pass
        {
            glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Geometry Pass");
            _gBuffer->Bind();
            glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _model->Draw(_gBufferShader, modelTransform);

            // Draw Skybox
            {
                _viewMatrix = glm::mat4(glm::mat3(_viewMatrix));
                _uniformBuffer->SetData(&_viewMatrix, sizeof(glm::mat4), 0);
                glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 2, -1, "Skybox");
                glDepthFunc(GL_LEQUAL);
                _skybox->Bind();
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                glDepthFunc(GL_LESS);
                glPopDebugGroup();
            }

            _gBuffer->Unbind();
            glPopDebugGroup();
        }

        // Lighting pass
        {
            glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Lighting Pass");
            //_screenFramebuffer->Bind();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Bind gbuffer textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _gBuffer->GetColorAttachmentId(0)); // Position
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, _gBuffer->GetColorAttachmentId(1)); // Normal
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, _gBuffer->GetColorAttachmentId(2)); // AlbedoSpec

            _lightingShader->Bind();
            _lightingShader->SetUniformInt("gPosition", 0);
            _lightingShader->SetUniformInt("gNormal", 1);
            _lightingShader->SetUniformInt("gAlbedoSpec", 2);

            // Pass light uniforms to shader
            for (const PointLight& light : _pointLights)
            {
                _lightingShader->SetUniformFloat3("u_light.position", light.position);
                _lightingShader->SetUniformFloat3("u_light.color", light.color);
            }

            // Render screen quad for lighting pass
            _screenVAO->Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            //_screenFramebuffer->Unbind();
            glPopDebugGroup();
        }

        

        // Screen pass
        {
            /*glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 2, -1, "Screen Pass");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            _screenShader->Bind();
            _screenVAO->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _screenFramebuffer->GetColorAttachmentId());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glPopDebugGroup();*/
        }
    }

    void DeferredRenderer::OnEvent(Event& e)
    {
        if (e.GetEventType() == EventType::WindowResize)
        {
            WindowResizeEvent& wre = (WindowResizeEvent&)e;
            _gBuffer->Resize(wre.GetWidth(), wre.GetHeight());
            _screenFramebuffer->Resize(wre.GetWidth(), wre.GetHeight());
        }
    }

    void DeferredRenderer::OnImGuiRender()
    {
        ImGui::Begin("Renderer");
        ImGui::ColorEdit4("Clear Color", &_clearColor.r);

        ImGui::Separator();

        ImGui::Text("Lights");
        if (ImGui::TreeNodeEx("Point Lights", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
        {
            for (int i = 0; i < _pointLights.size(); ++i)
            {
                if (ImGui::TreeNodeEx("Point Light - " + i, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
                {
                    ImGui::DragFloat3("Position", &_pointLights[i].position.x, 0.1f);
                    ImGui::ColorEdit3("Color", &_pointLights[i].color.x, 0.1f);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::Begin("Position color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(0), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::Begin("Normal color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(1), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::Begin("Albedo color buffer");
        ImGui::Image(_gBuffer->GetColorAttachmentId(2), ImVec2{ 256, 256 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();

        ImGui::Begin("Model");
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", &_modelPosition.x, 0.1f);
        ImGui::DragFloat3("Rotation", &_modelRotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &_modelScale.x, 0.1f);
        ImGui::Separator();
        _model->OnImGuiRender();
        ImGui::End();

        ImGui::End();
    }
}
