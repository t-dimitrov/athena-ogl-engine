//#include "Window.h"
//
//namespace Athena
//{
//	void GLFWErrorCallback(int code, const char* description)
//	{
//		PLR_ENGINE_ERROR("GLFW Error [{0}]: {1}", code, description);
//	}
//
//	Window::Window(const WindowDescriptor& desc)
//		: m_Descriptor(desc)
//	{
//		bool isGlfwInitialized = glfwInit();
//		PLR_ENGINE_ASSERT(isGlfwInitialized, "GLFW failed to initialize")
//
//			glfwSetErrorCallback(GLFWErrorCallback);
//
//		if (m_Descriptor.fullscreen)
//		{
//			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
//			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
//			m_WindowHandle = glfwCreateWindow(mode->width, mode->height, desc.title.c_str(), primaryMonitor, nullptr);
//		}
//		else
//		{
//			glfwWindowHint(GLFW_MAXIMIZED, desc.maximize ? GLFW_TRUE : GLFW_FALSE);
//			m_WindowHandle = glfwCreateWindow((int)desc.width, (int)desc.height, desc.title.c_str(), nullptr, nullptr);
//		}
//
//		if (!m_WindowHandle)
//		{
//			glfwTerminate();
//			PLR_ENGINE_ASSERT(false, "Failed to create GLFW Window.");
//		}
//		glfwSetWindowUserPointer(m_WindowHandle, static_cast<void*>(this));
//
//		/* Window Callbacks */
//		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			WindowCloseEvent e;
//			self->DispatchEvent(e);
//			});
//
//		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			if (width == 0 && height == 0)
//			{
//				//TODO: WindowMinimizeEvent
//				return;
//			}
//
//			if (width == self->GetWidth() && height == self->GetHeight())
//			{
//				//TODO: WindowRestoreEvent
//				return;
//			}
//
//			self->UpdateWindowSize();
//
//			WindowResizeEvent e(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
//			self->DispatchEvent(e);
//			});
//
//		/* Input Callbacks */
//		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			switch (action)
//			{
//			case GLFW_PRESS:
//			{
//				KeyPressEvent e((KeyCode)key);
//				self->DispatchEvent(e);
//				break;
//			}
//
//			case GLFW_RELEASE:
//			{
//				KeyReleaseEvent e((KeyCode)key);
//				self->DispatchEvent(e);
//				break;
//			}
//
//			case GLFW_REPEAT:
//			{
//				KeyPressEvent e((KeyCode)key);
//				self->DispatchEvent(e);
//				break;
//			}
//			}
//			});
//
//		glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* window, uint32_t codepoint) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			KeyTypeEvent e((KeyCode)codepoint);
//			self->DispatchEvent(e);
//			});
//
//		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			switch (action)
//			{
//			case GLFW_PRESS:
//			{
//				MouseButtonPressEvent e((MouseCode)button);
//				self->DispatchEvent(e);
//				break;
//			}
//
//			case GLFW_RELEASE:
//			{
//				MouseButtonReleaseEvent e((MouseCode)button);
//				self->DispatchEvent(e);
//				break;
//			}
//			}
//			});
//
//		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xpos, double ypos) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			MouseMoveEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
//			self->DispatchEvent(e);
//			});
//
//		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xoffset, double yoffset) {
//			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//			MouseScrollEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
//			self->DispatchEvent(e);
//			});
//
//		PLR_ENGINE_INFO("GLFW version: {0}.{1}.{2}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
//	}
//
//	Window::~Window()
//	{
//		glfwDestroyWindow(m_WindowHandle);
//		glfwTerminate();
//	}
//
//	void Window::PollEvents()
//	{
//		glfwPollEvents();
//	}
//
//	void Window::Present()
//	{
//		glfwSwapBuffers(m_WindowHandle);
//	}
//
//	void Window::UpdateWindowSize()
//	{
//		int width, height;
//		glfwGetWindowSize(m_WindowHandle, &width, &height);
//
//		m_Descriptor.width = static_cast<uint32_t>(width);
//		m_Descriptor.height = static_cast<uint32_t>(height);
//	}
//
//	void Window::BindEventCallback(EventCallbackFunc eventCallbackFunc)
//	{
//		m_EventCallbackFunc = eventCallbackFunc;
//	}
//
//	void Window::DispatchEvent(Event& e)
//	{
//		if (m_EventCallbackFunc)
//		{
//			m_EventCallbackFunc(e);
//		}
//	}
//}
