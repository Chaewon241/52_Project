#pragma once
#include <d2d1.h>
#include <list>
#include <string>
#include <wincodec.h>
#include <unordered_map>
#include <vector>

class AnimationClip;
class Sprite;
struct FRAME_INFO;

class D2DRenderer
{
public:
	static ID2D1HwndRenderTarget* m_renderTarget;
	static D2D1_MATRIX_3X2_F m_CameraTransform;
	static D2D1_MATRIX_3X2_F m_ScreenTransform;

	

	ID2D1Factory* m_pD2DFactory;	

	ID2D1Factory* m_D2DFactory = NULL;
	ID2D1SolidColorBrush* m_pRedBrush = NULL;
	ID2D1SolidColorBrush* m_pBlackBrush = NULL;
	ID2D1SolidColorBrush* m_pBlueBrush = NULL;

	std::unordered_map<std::wstring, ID2D1Bitmap*> m_spriteSheets;

public:
	D2DRenderer();
	~D2DRenderer();
public:
	HRESULT Initialize();
	void LoadSpriteSheet(std::wstring spriteName,LPCWSTR filePath);
	void BeginRender();
	void DrawSprite(Sprite* sprite);
	void DrawAnimation(std::wstring clipName, FRAME_INFO aniName);
	void DrawRectangle(D2D1_RECT_F rect);
	void DrawEllipse(D2D1_POINT_2F point, float radiusX, float radiusY);
	void DrawCrossLine();

	void SetTransform(const D2D1_MATRIX_3X2_F& transMatrix);
	void SetCameraTransform(const D2D1_MATRIX_3X2_F& worldTrasnform) { m_CameraTransform = worldTrasnform; }
	void EndRender();
public:
	static D2DRenderer* m_hInstance;
};
