#pragma once
#include "Core/Window.h"
#include "Ref.h"

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
    };
}
