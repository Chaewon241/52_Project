#include "RenderManager.h"
#include "WinApp.h"

#pragma comment(lib, "msimg32.lib")

namespace catInWonderland
{
	HWND hWnd;

	HDC frontMemDC;
	HDC backMemDC;

	HBITMAP backBitmap = nullptr;

	RenderManager* RenderManager::instance = nullptr;
	RenderManager::RenderManager() 	{}
	RenderManager::~RenderManager() {}

	RenderManager* RenderManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new RenderManager();
		}
		return instance;
	}

	void RenderManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void RenderManager::Init()
	{
		hWnd = catInWonderland::WinApp::GetInstance()->GetWindow();

		frontMemDC = GetDC(hWnd);					// 윈도우에 그림을 그리는 DC 얻기
		backMemDC = CreateCompatibleDC(frontMemDC);	// frontMemDC와 호환되는 DC를 생성

		int width = WinApp::GetInstance()->GetWidth();
		int height = WinApp::GetInstance()->GetHeight();

		backBitmap = CreateCompatibleBitmap(frontMemDC, catInWonderland::WinApp::GetInstance()->GetWidth(),
			catInWonderland::WinApp::GetInstance()->GetHeight());
	}

	void RenderManager::ClearScreen()
	{
		// 지정된 장치 컨텍스트에 현재 선택된 브러시를 사용하여 지정된 사각형을 그립니다.
		// 브러시 색상과 표면 색상은 지정된 래스터 작업을 사용하여 결합됩니다.
		::PatBlt(backMemDC, 0, 0, catInWonderland::WinApp::GetInstance()->GetWidth(),
			catInWonderland::WinApp::GetInstance()->GetHeight(), WHITENESS);

		// 이거로 스크린 클리어 못하나?
		// DeleteDC(backMemDC);
	}

	void RenderManager::BeginDraw()
	{
		RenderManager::GetInstance()->ClearScreen();

		::SelectObject(backMemDC, backBitmap);
	}

	// 갑자기 COLORREF가 안 먹어서 RGB로 줬음
	void RenderManager::CreatePlayer(int x, int y, int radius)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		Ellipse(backMemDC, x - radius, y - radius, x + radius, y + radius);

		DeleteObject(hPen);
		DeleteObject(hBrush);
	}

	void RenderManager::EndDraw()
	{
		::BitBlt(frontMemDC, 0, 0, catInWonderland::WinApp::GetInstance()->GetWidth(), 
			catInWonderland::WinApp::GetInstance()->GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	void RenderManager::ReleaseRender()
	{
		// 얘로 release 못하나?
		// ::ReleaseDC(hWnd, frontMemDC);

	}
}