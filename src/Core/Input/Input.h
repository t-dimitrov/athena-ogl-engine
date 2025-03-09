#pragma once
#include "KeyCode.h"
#include "MouseCode.h"

namespace Athena
{
	class Input
	{
	public:
		static bool GetKeyPressed(KeyCode keycode);
		static bool GetMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
	};
}
