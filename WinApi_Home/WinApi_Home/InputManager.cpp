#include "InputManager.h"
#include "WinApp.h"

namespace catInWonderland
{
	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	InputManager* InputManager::instance = nullptr;
	InputManager::InputManager() {}
	InputManager::~InputManager() {}

	InputManager* InputManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputManager();
		}
		return instance;
	}

	void InputManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void InputManager::ResetInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
		}
	}

	void InputManager::KeyDown(unsigned int key)
	{
		isKeyDown[key] = true;
		isKey[key] = true;
	}

	void InputManager::KeyUp(unsigned int key)
	{
		isKeyUp[key] = true;
		isKey[key] = false;
	}

	bool InputManager::IsKeyDown(unsigned int key)
	{
		return isKeyDown[key];
	}

	bool InputManager::IsKeyUp(unsigned int key)
	{
		return isKeyUp[key];
	}

	void InputManager::Init()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
			isKey[i] = false;
		}
	}
}