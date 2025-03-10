#include "Application.h"

#include "Event/KeyEvents.h"

#include <glad/glad.h>

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
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        while (_isRunning)
        {
            _window->PollEvents();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

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
