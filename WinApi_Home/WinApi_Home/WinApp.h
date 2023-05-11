#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

constexpr int SCREEN_START_LEFT = 10;
constexpr int SCREEN_START_TOP = 10;
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

class WinApp
{
public:

	WinApp() = default;
	~WinApp() = default;

	void Initialize(HINSTANCE hInstance);

	void Run();

	void Finalize();

	HINSTANCE GetInstance() const { return m_hInstance; }

	HWND GetWindow() const { return m_hWnd; }

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE m_hInstance = { 0 }; 
	HWND m_hWnd = { 0 };		

	int width = 1024;
	int height = 768;
};

namespace global
{
	const WinApp& GetWinApp();
}

