#include "pch.h"
#include "InputManager.h"

InputManager* InputManager::m_pInputInstance = nullptr;

void InputManager::Initialize()
{
}

void InputManager::Update()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (!isKeyPressed[i])
			{
				isKeyPressed[i] = true;
				prevKey = i;
			}
		}
		else
		{
			isKeyPressed[prevKey] = false;
		}
	}
}

