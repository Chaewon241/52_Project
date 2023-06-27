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
	void SetTransform(const D2D1_MATRIX_3X2_F& transMatrix);
	void DrawRectangle(float x1, float y1, float x2, float y2);
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};