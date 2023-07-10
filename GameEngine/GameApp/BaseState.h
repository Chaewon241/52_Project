#pragma once
class GameObject;

class BaseState
{
public:
	BaseState* m_pBaseStateInstance;
public:
	virtual void OnStateEnter() {};
	virtual void OnStateUpdate() {};
	virtual void OnStateExit() {};
};
