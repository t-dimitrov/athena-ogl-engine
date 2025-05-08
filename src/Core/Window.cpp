#include "Window.h"
#include "Log.h"
#include "Core.h"

#include "Event/KeyEvents.h"
#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"

#include "imgui.h"

namespace Athena
{
	void GLFWErrorCallback(int code, const char* description)
	{
		LOG_ERROR("GLFW Error [{0}]: {1}", code, description);
	}

	Window::Window(const WindowDescriptor& desc)
		: _descriptor(desc)
		, _showImGuiDemo(false)
	{
		bool isGlfwInitialized = glfwInit();
		ATH_ASSERT(isGlfwInitialized, "GLFW failed to initialize")

		glfwSetErrorCallback(GLFWErrorCallback);

		if (_descriptor.fullscreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
			_windowHandle = glfwCreateWindow(mode->width, mode->height, desc.title.c_str(), primaryMonitor, nullptr);
		}
		else
		{
			glfwWindowHint(GLFW_MAXIMIZED, desc.maximize ? GLFW_TRUE : GLFW_FALSE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			_windowHandle = glfwCreateWindow((int)desc.width, (int)desc.height, desc.title.c_str(), nullptr, nullptr);
		}

		if (!_windowHandle)
		{
			glfwTerminate();
			ATH_ASSERT(false, "Failed to create GLFW Window.");
		}
		glfwSetWindowUserPointer(_windowHandle, static_cast<void*>(this));

		if (desc.maximize)
		{
			UpdateWindowSize();
		}

		/* Window Callbacks */
		glfwSetWindowCloseCallback(_windowHandle, [](GLFWwindow* window) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent e;
			self->DispatchEvent(e);
			});

		glfwSetWindowSizeCallback(_windowHandle, [](GLFWwindow* window, int width, int height) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (width == 0 && height == 0)
			{
				WindowMinimizeEvent e;
				self->DispatchEvent(e);
				return;
			}

			if (width == self->GetWidth() && height == self->GetHeight())
			{
				WindowMaximizeEvent e;
				self->DispatchEvent(e);
				return;
			}

			self->UpdateWindowSize();

			WindowResizeEvent e(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
			self->DispatchEvent(e);
			});

		/* Input Callbacks */
		glfwSetKeyCallback(_windowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressEvent e((KeyCode)key);
				self->DispatchEvent(e);
				break;
			}

			case GLFW_RELEASE:
			{
				KeyReleaseEvent e((KeyCode)key);
				self->DispatchEvent(e);
				break;
			}

			case GLFW_REPEAT:
			{
				KeyPressEvent e((KeyCode)key);
				self->DispatchEvent(e);
				break;
			}
			}
			});

		glfwSetCharCallback(_windowHandle, [](GLFWwindow* window, uint32_t codepoint) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			KeyTypeEvent e((KeyCode)codepoint);
			self->DispatchEvent(e);
			});

		glfwSetMouseButtonCallback(_windowHandle, [](GLFWwindow* window, int button, int action, int mods) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent e((MouseCode)button);
				self->DispatchEvent(e);
				break;
			}

			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent e((MouseCode)button);
				self->DispatchEvent(e);
				break;
			}
			}
			});

		glfwSetCursorPosCallback(_windowHandle, [](GLFWwindow* window, double xpos, double ypos) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			MouseMoveEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
			self->DispatchEvent(e);
			});

		glfwSetScrollCallback(_windowHandle, [](GLFWwindow* window, double xoffset, double yoffset) {
			auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			MouseScrollEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
			self->DispatchEvent(e);
			});

		LOG_INFO("GLFW version: {0}.{1}.{2}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

		glfwMakeContextCurrent(_windowHandle);
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!result)
		{
			LOG_ERROR("Failed to load GL loader ({})", result);
		}

		_imguiRenderer = Ref<ImGuiRenderer>::Create();
		_imguiRenderer->Init(_windowHandle);
	}

	Window::~Window()
	{
		_imguiRenderer->Shutdown();
		glfwDestroyWindow(_windowHandle);
		glfwTerminate();
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Present()
	{
		glfwSwapBuffers(_windowHandle);
	}

	void Window::OnImGuiBeginFrame()
	{
		_imguiRenderer->BeginFrame();

		ImGui::Begin("Window Settings");

		ImGui::Checkbox("Show demo window", &_showImGuiDemo);
		if (_showImGuiDemo)
			ImGui::ShowDemoWindow(&_showImGuiDemo);

		ImGui::End();
	}

	void Window::OnImGuiEndFrame()
	{
		_imguiRenderer->EndFrame(_windowHandle);
	}

	void Window::UpdateWindowSize()
	{
		int width, height;
		glfwGetWindowSize(_windowHandle, &width, &height);

		_descriptor.width = static_cast<uint32_t>(width);
		_descriptor.height = static_cast<uint32_t>(height);
	}

	void Window::BindEventCallback(EventCallbackFunc eventCallbackFunc)
	{
		_eventCallbackFunc = eventCallbackFunc;
	}

	void Window::DispatchEvent(Event& e)
	{
		if (_eventCallbackFunc)
		{
			_eventCallbackFunc(e);
		}
	}
}
