#include "InputSystem.h"

#include <Windows.h>

void InputSystem::Initialize()
{
}

void InputSystem::Update()
{

}

bool InputSystem::isKeyPressed(char key) const
{
    return GetAsyncKeyState(key) & 0x8000;
}
