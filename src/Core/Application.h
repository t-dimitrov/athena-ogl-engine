#pragma once
#include "Ref.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

namespace Athena
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void OnEvent(Event& e);

    private:
        Ref<Window> _window;
        bool _isRunning = true;

        Ref<Renderer> _renderer;
    };
}
