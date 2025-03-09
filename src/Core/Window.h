//#pragma once
//#include "Ref.h"
//#include <functional>
//#include <string>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//namespace Athena
//{
//	struct WindowDescriptor
//	{
//		std::string title = "Athena Window";
//		uint32_t width = 1600;
//		uint32_t height = 900;
//		bool fullscreen = false;
//		bool maximize = false;
//		bool vsync = true;
//	};
//
//	using EventCallbackFunc = std::function<void(Event&)>;
//
//	class Window : public RefCounted
//	{
//	public:
//		Window(const WindowDescriptor& desc);
//		~Window();
//
//		GLFWwindow* GetNativeWindow() const { return m_WindowHandle; }
//
//		void PollEvents();
//		void Present();
//
//		uint32_t GetWidth() const { return m_Descriptor.width; }
//		uint32_t GetHeight() const { return m_Descriptor.height; }
//
//		void UpdateWindowSize();
//
//		void BindEventCallback(EventCallbackFunc eventCallbackFunc);
//
//	private:
//		void DispatchEvent(Event& e);
//
//	private:
//		WindowDescriptor m_Descriptor;
//		GLFWwindow* m_WindowHandle;
//
//		EventCallbackFunc m_EventCallbackFunc;
//	};
//}
