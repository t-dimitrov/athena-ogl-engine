#include "Application.h"

#include "Event/KeyEvents.h"

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

        _renderer = Ref<Renderer>::Create();
        _renderer->Init();
    }

    Application::~Application()
    {
        _renderer->Shutdown();
        s_application = nullptr;
    }

    void Application::Run()
    {
        while (_isRunning)
        {
            _window->PollEvents();

            _editorCamera->OnUpdate(1.0f);

            _renderer->BeginFrame(_editorCamera);
            
            _window->OnImGuiBeginFrame();
            _editorCamera->OnImGuiRender();
            
            _renderer->OnImGuiRender();
            _renderer->EndFrame();

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
}
