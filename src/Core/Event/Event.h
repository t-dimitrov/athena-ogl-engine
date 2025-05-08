#pragma once
#include <string>

namespace Athena
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowMinimize, WindowMaximize, // Application
		KeyPress, KeyRelease, KeyType, // Input, Keyboard
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll // Input, Mouse
	};

	enum EventCategory : int8_t
	{
		EVENT_CATEGORY_NONE			= 0,

		EVENT_CATEGORY_APPLICATION	= 1 << 0,

		EVENT_CATEGORY_INPUT		= 1 << 1,
		EVENT_CATEGORY_KEYBOARD		= 1 << 2,
		EVENT_CATEGORY_MOUSE		= 1 << 3
	};

#define EVENT_CLASS_SET_TYPE(type) virtual EventType GetEventType() const override { return type; }
#define EVENT_CLASS_SET_NAME(name) virtual const char* GetName() const override { return name; }
#define EVENT_CLASS_SET_CATEGORY(category) virtual int8_t GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual int8_t GetCategoryFlags() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return static_cast<bool>(GetCategoryFlags() & category);
		}

	public:
		bool handled = false;
	};
}
