#pragma once
#include "Event.h"
#include "Core/Input/MouseCode.h"

#include <sstream>

namespace Athena
{
	class MouseButtonEvent : public Event
	{
	protected:
		MouseButtonEvent(MouseCode mousecode)
			: _mouseCode(mousecode)
		{}
		MouseCode _mouseCode;

	public:
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << _mouseCode;
			return ss.str();
		}

		MouseCode GetMouseCode() const { return _mouseCode; }
	};

	class MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(MouseCode mousecode)
			: MouseButtonEvent(mousecode)
		{}

		EVENT_CLASS_SET_TYPE(EventType::MouseButtonPress)
		EVENT_CLASS_SET_NAME("MouseButtonPressEvent")
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(MouseCode mousecode)
			: MouseButtonEvent(mousecode)
		{}

		EVENT_CLASS_SET_TYPE(EventType::MouseButtonRelease)
		EVENT_CLASS_SET_NAME("MouseButtonReleaseEvent")
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float xpos, float ypos)
			: _posX(xpos)
			, _posY(ypos)
		{}

		EVENT_CLASS_SET_TYPE(EventType::MouseMove)
		EVENT_CLASS_SET_NAME("MouseMoveEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": (" << _posX << ", " << _posY << ")";
			return ss.str();
		}

		std::pair<float, float> GetPosition() const { return { _posX, _posY }; }

	private:
		float _posX;
		float _posY;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xoffset, float yoffset)
			: _offsetX(xoffset)
			, _offsetY(yoffset)
		{}

		EVENT_CLASS_SET_TYPE(EventType::MouseScroll)
		EVENT_CLASS_SET_NAME("MouseScrollEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": (" << _offsetX << ", " << _offsetY << ")";
			return ss.str();
		}

		std::pair<float, float> GetOffset() const { return { _offsetX, _offsetY }; }

	private:
		float _offsetX;
		float _offsetY;
	};
}
