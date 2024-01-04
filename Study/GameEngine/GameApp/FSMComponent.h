#pragma once
#include "Component.h"

class GameObject;
class FiniteStateMachine;

class FSMComponent :
    public Component
{
public:
    FSMComponent(GameObject* gameObject) : Component(gameObject) {};
    ~FSMComponent() {};

public:
    void Update()
    {
    }
protected:
    

};