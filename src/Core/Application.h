#pragma once
#include "Ref.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"

namespace Athena
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void OnEvent(Event& e);
        void OnImGuiRender();

        inline Ref<Window> GetWindow() const { return _window; }

        static Application& Instance() { return *s_application; }

    private:
        Ref<Window> _window;
        bool _isRunning = true;

        Ref<EditorCamera> _editorCamera;

        Ref<MaterialLibrary> _materialLibrary;
        Ref<TextureLibrary> _textureLibrary;

        //Ref<Renderer> _renderer;
        Ref<DeferredRenderer> _renderer;

    private:
        static Application* s_application;
    };
}
