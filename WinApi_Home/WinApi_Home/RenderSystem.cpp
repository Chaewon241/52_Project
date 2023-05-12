#include "WinApp.h"
#include "RenderSystem.h"

#pragma comment(lib, "msimg32.lib")

namespace render
{
	HWND hWnd;

	HDC frontMemDC;
	HDC backMemDC;

	HBITMAP backBitmap = nullptr;

	void InitRender()
	{
		hWnd = global::GetWinApp().GetWindow();

		frontMemDC = GetDC(hWnd);					// 윈도우에 그림을 그리는 DC 얻기
		backMemDC = CreateCompatibleDC(frontMemDC);	// frontMemDC와 호환되는 DC를 생성

		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	void ClearScreen()
	{
		// 지정된 장치 컨텍스트에 현재 선택된 브러시를 사용하여 지정된 사각형을 그립니다.
		// 브러시 색상과 표면 색상은 지정된 래스터 작업을 사용하여 결합됩니다.
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);

		// 이거로 스크린 클리어 못하나?
		// DeleteDC(backMemDC);
	}

	void BeginDraw()
	{
		ClearScreen();

		::SelectObject(backMemDC, backBitmap);
	}

	void CreatePlayer(int x, int y, int radius, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Ellipse(backMemDC, x - radius, y - radius, x + radius, y + radius);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);
	}

	void EndDraw()
	{
		::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	void ReleaseRender()
	{
		// 얘로 release 못하나?
		// ::ReleaseDC(hWnd, frontMemDC);

	}
}