#pragma once
#include <d2d1.h>
#include <list>

class D2DRenderer
{
private:
	static ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1Factory* m_pD2DFactory;	

	ID2D1Factory* m_D2DFactory = NULL;
	ID2D1SolidColorBrush* m_pRedBrush = NULL;
	ID2D1SolidColorBrush* m_pBlackBrush = NULL;
	ID2D1SolidColorBrush* m_pBlueBrush = NULL;

	//std::list<std::pair<std::wstring, AnimationAsset*>> m_SharingAnimationAssets;

public:
	D2DRenderer();
	~D2DRenderer();
public:
	HRESULT Initialize();
	void BeginRender();
	void SetTransform(const D2D1_MATRIX_3X2_F& transMatrix);
	void DrawRectangle(float x1, float y1, float x2, float y2);
	void DrawEllipse(D2D1_POINT_2F point, float radiusX, float radiusY);
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};