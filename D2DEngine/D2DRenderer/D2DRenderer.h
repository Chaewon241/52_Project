#pragma once
#include <list>

class AnimationAsset;

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();
	HRESULT Initialize();
	static ID2D1HwndRenderTarget* m_pD2DRenderTarget;
	static D2DRenderer* m_Instance;
public:
	ID2D1Factory* m_pD2DFactory = NULL;				// 기본 D2D사용을 위한 Factory의 인터페이스 포인터

	ID2D1SolidColorBrush* g_pGrayBrush = NULL;		// 렌더타겟에서생성한 브러시의 인터페이스 포인터
	ID2D1SolidColorBrush* g_pBlackBrush = NULL;
	IDWriteFactory* g_pDWriteFactory = NULL;		// 텍스트 출력을 위한 객체 인터페이스 포인터
	IDWriteTextFormat* g_pDWriteTextFormat = NULL;
	IWICImagingFactory* m_pIWICImagingFactory = NULL;	// WIC 인터페이스

	std::list < std::pair<std::wstring, ID2D1Bitmap*>> m_SharingD2DBitmaps;
	std::list<std::pair<std::wstring, AnimationAsset*>> m_SharingAnimationAssets;
public:
	HRESULT CreateD2DBitmapFromFile(std::wstring strFilePath, ID2D1Bitmap** pID2D1Bitmap);
	void ReleaseD2DBitmapFromFile(ID2D1Bitmap* pID2D1Bitmap);
	void ReleaseAnimationAsset(AnimationAsset* pAnimationAsset);
};