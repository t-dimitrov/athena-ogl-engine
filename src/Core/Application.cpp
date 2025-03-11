#include "Application.h"

#include "Event/KeyEvents.h"

namespace Athena
{
    Application::Application()
    {
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

        _renderer = Ref<Renderer>::Create();
        _renderer->Init();
    }

    Application::~Application()
    {
        _renderer->Shutdown();
    }

    void Application::Run()
    {
        while (_isRunning)
        {
            _window->PollEvents();

            _renderer->Render();

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
    }
}
