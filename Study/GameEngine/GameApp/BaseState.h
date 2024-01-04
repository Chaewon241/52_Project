#pragma once
//#include "GameObject.h"

class GameObject;

class BaseState //: public GameObject
{
public:
	BaseState();
public:
	virtual void OnStateEnter() {};
	virtual void OnStateUpdate() {};
	virtual void OnStateExit() {};
};
