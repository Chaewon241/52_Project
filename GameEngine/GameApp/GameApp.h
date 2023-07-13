#pragma once

#include <Windows.h>
#include "../GameApp/D2DRenderer.h"

#define MAX_LOADSTRING 100

class D2DRenderer;
class TimeManager;

// 게임의 루프를 담당
class GameApp
{
private:
	D2D_SIZE_U m_ClientSize;

public:
	GameApp(HINSTANCE hInstace);
	virtual ~GameApp();

	virtual bool Initialize();
	void Loop();
	virtual void Update();
	virtual void Render();
	D2D_SIZE_U GetClientSize() { return m_ClientSize; }

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
public:
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