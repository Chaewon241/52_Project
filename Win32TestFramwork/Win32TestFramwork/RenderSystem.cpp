#include "WinApp.h"
#include "RenderSystem.h"

// 특정 라이브러리를 명시적으로 추출
#pragma comment(lib, "msimg32.lib")

namespace render
{
	HWND hWnd;

	// 더블 버퍼링을 위해 HDC를 두개로 함
	HDC frontMemDC;	// 앞면 DC
	HDC backMemDC;	// 뒷면 DC

	HBITMAP backBitmap = nullptr;

	void InitRender()
	{
		// 핸들 가져오기
		hWnd = global::GetWinApp().GetWindow();

		// 더블 버퍼링을 위해 앞면은 GetDc
		frontMemDC = GetDC(hWnd);
		// 뒷면은 앞면 어쩌구, DC 또 만들기
		backMemDC = CreateCompatibleDC(frontMemDC);
		// 그리기 위한 도화지(도화지 크기, 화면 크기)
		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	void ClearScreen()
	{
		// 뒷면 지워주기
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);
	}

	void BeginDraw()
	{
		ClearScreen();
		// 도화지 준비
		::SelectObject(backMemDC, backBitmap);
	}

	void EndDraw()
	{
		// 도화지에 그림 그리고(BitBlt) front에 보내주기
		::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	// 더이상 필요 없으면 release
	void ReleaseRender()
	{
		// 도화지 지워주고
		DeleteObject(backBitmap);
		// 화면 지워주고
		DeleteDC(backMemDC);

		ReleaseDC(hWnd, frontMemDC);
	}

	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 5, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		MoveToEx(backMemDC, x1, y1, NULL);

		LineTo(backMemDC, x2, y2);

		SelectObject(backMemDC, hOldPen);

		DeleteObject(hPen);
	}


	void DrawRect(int x, int y, int width, int height, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Rectangle(backMemDC, x, y, x + width, y + height);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);
	}


	void DrawCircle(int x, int y, int radius, COLORREF color)
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

	void DrawPolygon(POINT points[], int count, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Polygon(backMemDC, points, count);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);

	}

	void DrawText(int x, int y, const char* text, COLORREF color)
	{
		SetTextColor(backMemDC, color);

		TextOutA(backMemDC, x, y, text, strlen(text));
	}

	//여기서부터 추가
	void DrawBitmap(int x, int y, HBITMAP hBitmap)
	{
		// 백스크린 만들기
		HDC bitmapMemDC = CreateCompatibleDC(frontMemDC);
		// bitmapMemDC -> 비트맵을 위한 메모리 dc 만듦, 쓰고 나서 원래로 돌리고 싶을 때 씀
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(bitmapMemDC, hBitmap);

		BITMAP bm;
		// 비트맵에 대한 정보(사이즈 등)
		GetObject(hBitmap, sizeof(BITMAP), &bm);

		::BitBlt(backMemDC, x, y, bm.bmWidth, bm.bmHeight, bitmapMemDC, 0, 0, SRCCOPY);

		DeleteDC(bitmapMemDC);
	}

	HBITMAP LoadImdage(const char* path)
	{
		// 다 쓰면 release
		HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		return hBitmap;
	}

	void ReleaseImage(HBITMAP hBitmap)
	{
		DeleteObject(hBitmap);
	}

}