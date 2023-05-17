#pragma once

#include <Windows.h>

namespace catInWonderland
{
	class WinApp
	{
	public:
		WinApp() = delete;
		~WinApp() = delete;

		static int Run(HINSTANCE hInstance, const TCHAR* appName, UINT width, UINT height);

		static HWND GetWindow();
		static UINT GetWidth();
		static UINT GetHeight();
		static const char* GetPath();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private: 
		static void placeInCenterOfScreen(HWND window, DWORD style, DWORD exStyle);

	private:
		static HWND mHWnd;
		static UINT mWidth;
		static UINT mHeight;
		static char mPath[260];
	};
}