#include "Application.h"

#include "Event/KeyEvents.h"
#include <imgui.h>

namespace Athena
{
    Application* Application::s_application = nullptr;

    Application::Application()
    {
        s_application = this;

        WindowDescriptor desc = {
            "Athena Window",
            1600,
            900,
            false,
            false,
            true,
        };
        _window = Ref<Window>::Create(desc);
        _window->BindEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        _editorCamera = Ref<EditorCamera>::Create();
        _editorCamera->SetPerspective(45.0f, static_cast<float>(desc.width), static_cast<float>(desc.height), 0.1f, 1000.0f);

        _materialLibrary = Ref<MaterialLibrary>::Create();
        _materialLibrary->Init();
        _textureLibrary = Ref<TextureLibrary>::Create();
        _textureLibrary->Init();

        _renderer = Ref<Renderer>::Create();
        _renderer->Init();
    }

    Application::~Application()
    {
        _renderer->Shutdown();
        _textureLibrary->Release();
        _materialLibrary->Release();
        s_application = nullptr;
    }

    void Application::Run()
    {
        while (_isRunning)
        {
            _window->PollEvents();

            _editorCamera->OnUpdate(1.0f);

            _renderer->BeginFrame(_editorCamera);
            //_renderer->SubmitModel(_model, _shader, _modelPos, _modelScale, _modelRotation);
            _renderer->EndFrame();
            
            _window->OnImGuiBeginFrame();
                _editorCamera->OnImGuiRender();
                _renderer->OnImGuiRender();
                OnImGuiRender();
            _window->OnImGuiEndFrame();

            _window->Present();
        }
    }

    void Application::OnEvent(Event& e)
    {
        if (e.GetEventType() == EventType::WindowClose)
        {
            _isRunning = false;
        }

        if (e.GetEventType() == EventType::KeyPress)
        {
            KeyPressEvent& kpe = (KeyPressEvent&)e;
            if (kpe.GetKeyCode() == KeyCode::Escape)
            {
                _isRunning = false;
            }
        }

        _editorCamera->OnEvent(e);
        _renderer->OnEvent(e);
    }

    void Application::OnImGuiRender()
    {
        ImGui::Begin("Application");
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
                        ImGui::Image(material->albedoTexture->GetTextureId(), ImVec2{ 100.0f, 100.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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
    }
}
