#pragma once
#include "KeyCode.h"
#include "MouseCode.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Athena
{
	class Input
	{
	public:
		static bool GetKeyPressed(KeyCode keycode)
		{
			auto windowHandle = Application::Instance().GetWindow()->GetNativeWindow();

			auto state = glfwGetKey(windowHandle, static_cast<int32_t>(keycode));
			return state == GLFW_PRESS && state != GLFW_REPEAT;
		}

		static bool GetMouseButtonPressed(MouseCode button)
		{
			auto windowHandle = Application::Instance().GetWindow()->GetNativeWindow();

			auto state = glfwGetMouseButton(windowHandle, static_cast<int32_t>(button));
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		static std::pair<float, float> GetMousePosition()
		{
			auto windowHandle = Application::Instance().GetWindow()->GetNativeWindow();

			double xpos, ypos;
			glfwGetCursorPos(windowHandle, &xpos, &ypos);

			return std::make_pair(static_cast<float>(xpos), static_cast<float>(ypos));
		}
	};
}
