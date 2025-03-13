#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Ref.h"
#include "Event/Event.h"
#include "ImGui/ImGuiRenderer.h"

#include <functional>
#include <string>


namespace Athena
{
	struct WindowDescriptor
	{
		std::string title = "Athena Window";
		uint32_t width = 1600;
		uint32_t height = 900;
		bool fullscreen = false;
		bool maximize = false;
		bool vsync = true;
	};

	using EventCallbackFunc = std::function<void(Event&)>;

	class Window : public RefCounted
	{
	public:
		Window(const WindowDescriptor& desc);
		~Window();

		GLFWwindow* GetNativeWindow() const { return _windowHandle; }

		void PollEvents();
		void Present();

		void OnImGuiBeginFrame() const;
		void OnImGuiEndFrame() const;

		uint32_t GetWidth() const { return _descriptor.width; }
		uint32_t GetHeight() const { return _descriptor.height; }

		void UpdateWindowSize();

		void BindEventCallback(EventCallbackFunc eventCallbackFunc);

	private:
		void DispatchEvent(Event& e);

	private:
		WindowDescriptor _descriptor;
		GLFWwindow* _windowHandle;
		Ref<ImGuiRenderer> _imguiRenderer;

		EventCallbackFunc _eventCallbackFunc;
	};
}
