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

        inline Ref<Window> GetWindow() const { return _window; }

        static Application& Instance() { return *s_application; }

    private:
        Ref<Window> _window;
        bool _isRunning = true;

        Ref<EditorCamera> _editorCamera;
        Ref<Renderer> _renderer;

    private:
        static Application* s_application;
    };
}
