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

		frontMemDC = GetDC(hWnd);					// �����쿡 �׸��� �׸��� DC ���
		backMemDC = CreateCompatibleDC(frontMemDC);	// frontMemDC�� ȣȯ�Ǵ� DC�� ����

		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	void ClearScreen()
	{
		// ������ ��ġ ���ؽ�Ʈ�� ���� ���õ� �귯�ø� ����Ͽ� ������ �簢���� �׸��ϴ�.
		// �귯�� ����� ǥ�� ������ ������ ������ �۾��� ����Ͽ� ���յ˴ϴ�.
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);

		// �̰ŷ� ��ũ�� Ŭ���� ���ϳ�?
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
		// ��� release ���ϳ�?
		// ::ReleaseDC(hWnd, frontMemDC);

	}
}