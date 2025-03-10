#pragma once
#include "Ref.h"
#include "Event/Event.h"

#include <functional>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		uint32_t GetWidth() const { return _descriptor.width; }
		uint32_t GetHeight() const { return _descriptor.height; }

		void UpdateWindowSize();

		void BindEventCallback(EventCallbackFunc eventCallbackFunc);

	private:
		void DispatchEvent(Event& e);

	private:
		WindowDescriptor _descriptor;
		GLFWwindow* _windowHandle;

		EventCallbackFunc _eventCallbackFunc;
	};
}
