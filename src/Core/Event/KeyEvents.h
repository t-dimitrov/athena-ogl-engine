#pragma once
#include "Event.h"

#include "Core/Input/KeyCode.h"

#include <sstream>

namespace Athena
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(KeyCode keycode)
			: _keyCode(keycode) {}

		KeyCode _keyCode;

	public:
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_KEYBOARD)

		inline KeyCode GetKeyCode() const { return _keyCode; }
	};

	class KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(KeyCode keycode)
			: KeyEvent(keycode) 
		{}

		EVENT_CLASS_SET_TYPE(EventType::KeyPress)
		EVENT_CLASS_SET_NAME("KeyPressEvent")

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << _keyCode;
			return ss.str();
		}
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{}

		EVENT_CLASS_SET_TYPE(EventType::KeyRelease)
		EVENT_CLASS_SET_NAME("KeyReleaseEvent")

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << _keyCode;
			return ss.str();
		}
	};

	class KeyTypeEvent : public KeyEvent
	{
	public:
		KeyTypeEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{}

		EVENT_CLASS_SET_TYPE(EventType::KeyType)
		EVENT_CLASS_SET_NAME("KeyTypeEvent")

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << _keyCode;
			return ss.str();
		}
	};
}
