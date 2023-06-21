#pragma once

#include <Windows.h>

#define MAX_LOADSTRING 100

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Initialized();
	void Loop();
	virtual void Update();
	virtual void Render();

protected:
	WNDCLASSEXW m_wcex;
	HINSTANCE m_hInst;
	HWND m_hwnd;

	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	HACCEL m_hAccelTable;
};