#pragma once

#include <Windows.h>
#include "../GameApp/D2DRenderer.h"

#define MAX_LOADSTRING 100

class D2DRenderer;
class TimeManager;

// 게임의 루프를 담당
class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp();

	virtual bool Initialize();
	void Loop();
	virtual void Update();
	virtual void Render();
	
	static HWND m_hwnd;
	static GameApp* m_hInstance;

protected:
	WNDCLASSEXW m_wcex;
	HINSTANCE m_hInst;

	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	HACCEL m_hAccelTable;

	D2DRenderer m_D2DRenderer;
};