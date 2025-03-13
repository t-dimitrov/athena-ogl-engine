#pragma once
#include "Core/Ref.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Athena
{
    class ImGuiRenderer : public RefCounted
    {
    public:
        ImGuiRenderer();
        ~ImGuiRenderer();

        void Init(GLFWwindow* window);
        void Shutdown();

        void BeginFrame(bool showDemo = false) const;
        void EndFrame(GLFWwindow* window) const;
    };
}
