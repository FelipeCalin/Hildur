#pragma once

#include "Event.h"

#include <sstream>

#include <glm/glm.hpp>


namespace Hildur {


	class HILDUR_API WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) 
		{
		}


		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResize: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		unsigned int m_Width;
		unsigned int m_Height;
	};

	class HILDUR_API WindowMoveEvent : public Event
	{
	public:

		WindowMoveEvent(unsigned int xpos, unsigned int ypos)
			: m_PosX(xpos), m_PosY(xpos)
		{
		}

		inline glm::vec2 GetPos() const { return glm::vec2(m_PosX, m_PosY); }
		inline unsigned int GetPosX() const { return m_PosX; }
		inline unsigned int GetPosY() const { return m_PosY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMove: " << m_PosX << ", " << m_PosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		unsigned int m_PosX;
		unsigned int m_PosY;
	};

	class HILDUR_API WindowCloseEvent : public Event
	{
	public:

		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	class HILDUR_API AppTickEvent : public Event 
	{
	public:

		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	class HILDUR_API AppUpdateEvent : public Event
	{
	public:

		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	class HILDUR_API AppRenderEvent : public Event 
	{
	public:

		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};


}