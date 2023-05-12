#include "WinApp.h"
#include "InputSystem.h"

namespace input
{
	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	void ResetInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
		}
	}

	void KeyDown(unsigned int key)
	{
		isKeyDown[key] = true;
		isKey[key] = true;
	}

	void KeyUp(unsigned int key)
	{
		isKeyUp[key] = true;
		isKey[key] = false;
	}

	bool IsKeyDown(unsigned int key)
	{
		return isKeyDown[key];
	}

	bool IsKeyUp(unsigned int key)
	{
		return isKeyUp[key];
	}

	void InitInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
			isKey[i] = false;
		}
	}
}