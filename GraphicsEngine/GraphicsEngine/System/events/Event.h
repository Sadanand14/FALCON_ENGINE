#ifndef EVENT_H
#define EVENT_H

#include "framework.h"

#include <string>
#include <functional>




namespace events {

	//All current events are trigger or blocking. As in they will fire up the function call as soon
	//as one even is generated. We need to add buffering mechanism for events as we go ahead.
	//Currently I followed tutorials on TheChernoProject for basic implementation. 


	/*
	* Defines the all the event types.  
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};


	/*
	* Defines event category flags which could be used to separate out the events.
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	
	/*
	* Abstract event class which outlines minimum requirements for events.
	*/
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	/*
	* Event dispatcher which dispatches the events to provided function.
	*/
	class EventDispatcher
	{
		
	private:
		Event& m_Event;

		template<typename T>
		using EventFn = std::function<bool(T&)>;
	
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}



		/*
		* This is the function used to dispatch events. 
		* Example:
		*
		* EventDispatcher dispatcher(e); //Create the object
		* dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); //Call the dispatch method
		*/
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)& m_Event);
				return true;
			}
			return false;
		}
	
	};

}



#endif //EVENT_H