#include "WinApp.h"
#include "InputSystem.h"

namespace input
{
	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	MouseState curMouse;
	MouseState prevMouse;

	// 한번 발생하고 리셋
	void ResetInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
		}
	}

	// keydown은 이벤트 한번 발생
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

	bool IsKey(unsigned int key)
	{
		return isKey[key];
	}

	void InitMouse()
	{
		curMouse.x = global::GetWinApp().GetWidth() / 2;
		curMouse.y = global::GetWinApp().GetHeight() / 2;
		curMouse.wheel = 0;

		curMouse.left = false;
		curMouse.right = false;
		curMouse.middle = false;

		prevMouse = curMouse;

		// 지정된 화면 좌표로 커서를 이동
		SetCursorPos(curMouse.x, curMouse.y);
	}

	void UpdateMouse()
	{
		prevMouse = curMouse;

		POINT pt;
		// 화면 좌표에서 마우스 커서의 위치를 검색
		// pt 에다가 현재 마우스 커서 위치 넣어줌
		GetCursorPos(&pt);
		// 화면에서 지정된 지점의 화면 좌표를 클라이언트 영역 좌표로 변환
		ScreenToClient(global::GetWinApp().GetWindow(), &pt);

		curMouse.x = pt.x;
		curMouse.y = pt.y;
		curMouse.wheel = 0;

		curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
		curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
		curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
	}

	const MouseState& GetMouseState()
	{
		return curMouse;
	}

	const MouseState& GetPrevMouseState()
	{
		return prevMouse;
	}

	void InitInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
			isKey[i] = false;
		}
		InitMouse();
	}

	bool IsSame(const MouseState& a, const MouseState& b)
	{
		return a.x == b.x && a.y == b.y && a.wheel == b.wheel && a.left == b.left && a.right == b.right && a.middle == b.middle;
	}
}