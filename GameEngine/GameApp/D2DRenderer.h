#pragma once
#include <d2d1.h>
#include <list>
#include <string>
#include <wincodec.h>

class AnimationClip;

class D2DRenderer
{
private:
	static ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1Factory* m_pD2DFactory;	
	IWICImagingFactory* m_pImageFactory;

	ID2D1Factory* m_D2DFactory = NULL;
	ID2D1SolidColorBrush* m_pRedBrush = NULL;
	ID2D1SolidColorBrush* m_pBlackBrush = NULL;
	ID2D1SolidColorBrush* m_pBlueBrush = NULL;

	std::list<std::pair<std::wstring, ID2D1Bitmap*>> m_SharingBitmaps;
	std::list<std::pair<std::wstring, AnimationClip*>> m_SharingAnimationAssets;

public:
	D2DRenderer();
	~D2DRenderer();
public:
	HRESULT Initialize();
	HRESULT CreateSharedD2DBitmapFromFile(std::wstring strFilePath, ID2D1Bitmap** ppID2D1Bitmap);
	AnimationClip* CreateSharedAnimationAsset(std::wstring strFilePath);
	void BeginRender();
	void SetTransform(const D2D1_MATRIX_3X2_F& transMatrix);
	void DrawAnimation(AnimationClip* m_pAnimationAsset);
	void DrawRectangle(float x1, float y1, float x2, float y2);
	void DrawEllipse(D2D1_POINT_2F point, float radiusX, float radiusY);
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};