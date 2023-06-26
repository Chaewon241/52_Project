#pragma once
#include <d2d1.h>

class D2DRenderer abstract
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
	void BegindRender();
	void Render();
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};