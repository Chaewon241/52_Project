#pragma once

namespace input
{
	void InitInput();

	void ResetInput();

	void KeyDown(unsigned int key);

	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);

	bool IsKeyUp(unsigned int key);

	bool IsKey(unsigned int key);
}