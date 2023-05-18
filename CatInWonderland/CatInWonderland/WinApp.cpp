#define WIN32_LEAN_AND_MEAN

#include <cassert>
#include <cstdlib>
#include <string>
#include <SDKDDKVer.h>

#include "GameCore.h"
#include "SoundManager.h"
#include "WinApp.h"


namespace catInWonderland
{
	HWND WinApp::mHWnd;
	UINT WinApp::mWidth;
	UINT WinApp::mHeight;
	char WinApp::mPath[260];

	int WinApp::Run(HINSTANCE hInstance, const TCHAR* appName, UINT width, UINT height)
	{
		mWidth = width;
		mHeight = height;

		WNDCLASS wndClass;
		ZeroMemory(&wndClass, sizeof(WNDCLASS));

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.hInstance = hInstance;
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wndClass.lpszClassName = appName;
		wndClass.lpfnWndProc = WinApp::WndProc;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);

		RegisterClass(&wndClass);

		RECT rect{ 0, 0, mWidth, mHeight };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		
		mHWnd = CreateWindow(
			appName, appName, 
			WS_OVERLAPPED | WS_SYSMENU,
			rect.top, rect.left,
			rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL,
			hInstance, NULL);

		ShowWindow(mHWnd, SW_SHOWNORMAL);
		UpdateWindow(mHWnd);

		MSG msg = { 0, };
		GameCore::GetInstance();
		
		// sound test
		SoundManager::GetInstance()->Init();
		SoundManager::GetInstance()->PlayMusic(SOUND_LIST::SOUND_LIST_BGM1, SOUND_CHANNEL::SOUND_CHANNEL_BGM, 0.5f);
		SoundManager::GetInstance()->PlayMusic(SOUND_LIST::SOUND_LIST_BGM2, SOUND_CHANNEL::SOUND_CHANNEL_BGM, 0.5f);

		// ¿©±â¿¡ path
		WinApp::GetPath();

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
			else
			{
				GameCore::GetInstance()->Frame();
			}
		}

		GameCore::DestroyInstance();

		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK WinApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			placeInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void WinApp::placeInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT clientRect;
		GetClientRect(window, &clientRect);

		int clientWidth = clientRect.right - clientRect.left;
		int clientHeight = clientRect.bottom - clientRect.top;

		SetWindowPos(window, NULL,
			screenWidth / 2 - clientWidth / 2,
			screenHeight / 2 - clientHeight / 2,
			clientWidth, clientHeight, 0
		);
	}

	HWND WinApp::GetWindow()
	{
		return mHWnd;
	}

	UINT WinApp::GetWidth()
	{
		return mWidth;
	}

	UINT WinApp::GetHeight()
	{
		return mHeight;
	}

	const char* WinApp::GetPath()
	{
		/*char highFolder[50] = "../../CatInWonderland/resources/";
		LPCSTR Filename = "";
		strcat(highFolder, Filename);
		OutputDebugStringA(highFolder);*/

		if (GetCurrentDirectoryA(MAX_PATH, mPath) > 0)
		{
			int str_len = strlen(mPath);

			OutputDebugStringA(("\nnum = " + std::to_string(str_len) + "\n").c_str());

			int highFolderIdx = str_len;
			for (highFolderIdx; highFolderIdx >= 0; highFolderIdx--)
			{
				if (mPath[highFolderIdx] == '\\')
				{
					mPath[highFolderIdx] = '\0';
					OutputDebugStringA(mPath);
					break;
				}
			}
		}

		return mPath;
	}
}