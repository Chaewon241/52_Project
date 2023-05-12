#include "WinApp.h"
#include "RenderSystem.h"

// Ư�� ���̺귯���� ��������� ����
#pragma comment(lib, "msimg32.lib")

namespace render
{
	HWND hWnd;

	// ���� ���۸��� ���� HDC�� �ΰ��� ��
	HDC frontMemDC;	// �ո� DC
	HDC backMemDC;	// �޸� DC

	HBITMAP backBitmap = nullptr;

	void InitRender()
	{
		// �ڵ� ��������
		hWnd = global::GetWinApp().GetWindow();

		// ���� ���۸��� ���� �ո��� GetDc
		frontMemDC = GetDC(hWnd);
		// �޸��� �ո� ��¼��, DC �� �����
		backMemDC = CreateCompatibleDC(frontMemDC);
		// �׸��� ���� ��ȭ��(��ȭ�� ũ��, ȭ�� ũ��)
		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	void ClearScreen()
	{
		// �޸� �����ֱ�
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);
	}

	void BeginDraw()
	{
		ClearScreen();
		// ��ȭ�� �غ�
		::SelectObject(backMemDC, backBitmap);
	}

	void EndDraw()
	{
		// ��ȭ���� �׸� �׸���(BitBlt) front�� �����ֱ�
		::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	// ���̻� �ʿ� ������ release
	void ReleaseRender()
	{
		// ��ȭ�� �����ְ�
		DeleteObject(backBitmap);
		// ȭ�� �����ְ�
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

	//���⼭���� �߰�
	void DrawBitmap(int x, int y, HBITMAP hBitmap)
	{
		// �齺ũ�� �����
		HDC bitmapMemDC = CreateCompatibleDC(frontMemDC);
		// bitmapMemDC -> ��Ʈ���� ���� �޸� dc ����, ���� ���� ������ ������ ���� �� ��
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(bitmapMemDC, hBitmap);

		BITMAP bm;
		// ��Ʈ�ʿ� ���� ����(������ ��)
		GetObject(hBitmap, sizeof(BITMAP), &bm);

		::BitBlt(backMemDC, x, y, bm.bmWidth, bm.bmHeight, bitmapMemDC, 0, 0, SRCCOPY);

		DeleteDC(bitmapMemDC);
	}

	HBITMAP LoadImdage(const char* path)
	{
		// �� ���� release
		HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		return hBitmap;
	}

	void ReleaseImage(HBITMAP hBitmap)
	{
		DeleteObject(hBitmap);
	}

}