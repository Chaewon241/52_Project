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

		frontMemDC = GetDC(hWnd);					// �����쿡 �׸��� �׸��� DC ���
		backMemDC = CreateCompatibleDC(frontMemDC);	// frontMemDC�� ȣȯ�Ǵ� DC�� ����

		int width = WinApp::GetInstance()->GetWidth();
		int height = WinApp::GetInstance()->GetHeight();

		backBitmap = CreateCompatibleBitmap(frontMemDC, catInWonderland::WinApp::GetInstance()->GetWidth(),
			catInWonderland::WinApp::GetInstance()->GetHeight());
	}

	void RenderManager::ClearScreen()
	{
		// ������ ��ġ ���ؽ�Ʈ�� ���� ���õ� �귯�ø� ����Ͽ� ������ �簢���� �׸��ϴ�.
		// �귯�� ����� ǥ�� ������ ������ ������ �۾��� ����Ͽ� ���յ˴ϴ�.
		::PatBlt(backMemDC, 0, 0, catInWonderland::WinApp::GetInstance()->GetWidth(),
			catInWonderland::WinApp::GetInstance()->GetHeight(), WHITENESS);

		// �̰ŷ� ��ũ�� Ŭ���� ���ϳ�?
		// DeleteDC(backMemDC);
	}

	void RenderManager::BeginDraw()
	{
		RenderManager::GetInstance()->ClearScreen();

		::SelectObject(backMemDC, backBitmap);
	}

	// ���ڱ� COLORREF�� �� �Ծ RGB�� ����
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
		// ��� release ���ϳ�?
		// ::ReleaseDC(hWnd, frontMemDC);

	}
}