#pragma once

#include "Singleton.h"
#include "Event.h"

#include <list>
#include <map>


enum class eEventType
{
	CHANGE_WORLD,
	END
};

class EventListener;

/// <summary>
///	1. RegisterListener()�� �̺�ƮŸ�԰� �����ʸ� ���
///	2. SendEvent()�� �̺�Ʈ�� �����ϰ� EventManager.Update()���� ������ m_EventList�� �߰�
///	3. Update()���� �̺�Ʈ Execute
/// </summary>
class EventManager
	:public Singleton<EventManager>
{
public:
	// �ش� eventType���� listener ���, ����
	void RegisterListener(eEventType eventType, EventListener* listener);
	void UnregisterListener(eEventType eventType, EventListener* listener);

	// �ش� listener�� ��ϵ� ��� �̺�Ʈ ����
	void UnregisterAll(EventListener* listener);

	void Initialize();
	void Update();
	void Finalize();

	/// �����ʷ� ��ϵ� ��� ��ü�� �̺�Ʈ�� �����ϱ� ���Ѵٸ� EventListener�� nullptr�� Send
	template <typename... Args>
	void SendEvent(eEventType eventId, EventListener* listener, Args... args)
	{
		Event newEvent(args...);
		newEvent.eventID = eventId;
		newEvent.eventListener = listener;

		m_eventList.emplace_back(newEvent);
	}

private:
	bool IsRegistered(eEventType eventType, EventListener* listener);
	void ExecuteEvent(Event* event);


private:
	std::multimap<eEventType, EventListener*> m_wholeEvents;	//  �ش� Ÿ������ ��ϵ� ������ ���
	std::list<Event> m_eventList;	// ó���Ǿ�� �ϴ� �̺�Ʈ ť
};

