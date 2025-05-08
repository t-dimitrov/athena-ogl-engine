#pragma once
#include "Event.h"

#include <sstream>

namespace Athena
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_SET_TYPE(EventType::WindowClose)
		EVENT_CLASS_SET_NAME("WindowCloseEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: _width(width)
			, _height(height) 
		{}

		EVENT_CLASS_SET_TYPE(EventType::WindowResize)
		EVENT_CLASS_SET_NAME("WindowResizeEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_APPLICATION)

		virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << GetName() << ": (" << _width << ", " << _height << ")";

			return ss.str();
		}

		uint32_t GetWidth() const { return _width; }
		uint32_t GetHeight() const { return _height; }

	private:
		uint32_t _width;
		uint32_t _height;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent() {}
		EVENT_CLASS_SET_TYPE(EventType::WindowMinimize)
		EVENT_CLASS_SET_NAME("WindowMinimizeEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent() {}
		EVENT_CLASS_SET_TYPE(EventType::WindowMaximize)
		EVENT_CLASS_SET_NAME("WindowMaximizeEvent")
		EVENT_CLASS_SET_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};
}
