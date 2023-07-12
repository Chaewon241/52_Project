#pragma once

class InputManager
{
public:
	void Initialize();
	void Update();
	inline void OnKeyDown(UINT keyCode);
	inline void OnKeyUp(UINT keyCode);
public:
	static InputManager* m_pInputInstance;
private:
	bool isKeyPressed[256] = { false };
	int prevKey;
};

inline void InputManager::OnKeyDown(UINT keyCode)
{
	isKeyPressed[keyCode] = true;
}

inline void InputManager::OnKeyUp(UINT keyCode)
{
	isKeyPressed[keyCode] = false;
}