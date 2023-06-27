#pragma once
#include <d2d1.h>

class D2DRenderer
{
private:
	static ID2D1HwndRenderTarget* m_RenderTarget;
	ID2D1Factory* m_D2DFactory = NULL;
	ID2D1SolidColorBrush* m_pRedBrush = NULL;
	ID2D1SolidColorBrush* m_pBlackBrush = NULL;
	ID2D1SolidColorBrush* m_pBlueBrush = NULL;

public:
	D2DRenderer();
	~D2DRenderer();
	HRESULT Initialize();
	void BeginRender();
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};