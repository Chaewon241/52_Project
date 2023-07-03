#pragma once
#include <vector>

struct FRAME_INFO
{
	D2D1_RECT_F SourceRect;
	float		RenderTime;

	FRAME_INFO(float x, float y, float width, float height, float time)
	{
		SourceRect.left = x;
		SourceRect.top = y;
		SourceRect.right = x + width;
		SourceRect.bottom = y + height;
		RenderTime = time;
	}
	FRAME_INFO(D2D1_RECT_F rect, float time)
	{
		SourceRect = rect;
		RenderTime = time;
	}
};

// 하나의 동작에 대한 정보
struct ANIMATION_INFO
{
	std::wstring m_Name;
	std::vector<FRAME_INFO> m_Frames;
	bool m_Loop;

	ANIMATION_INFO()
		:m_Loop(false)
	{

	}
};

class AnimationClip
{
private:
	std::vector<FRAME_INFO> m_AnimationClips;

public:
	AnimationClip()	{}
	~AnimationClip() {}
public:
	void SetD2DBitmap(const WCHAR* szFilePath);
public:
	ID2D1Bitmap* m_pBitmap;
	std::wstring m_BitmapFilePath;

	std::vector<ANIMATION_INFO> m_Animations;
};

/*
FRAME_INFO( 28, 36, 103, 84, 0.1f));
FRAME_INFO(148, 36,  86, 84, 0.1f));
FRAME_INFO(255, 34,  87, 86, 0.1f));
FRAME_INFO(363, 32,  76, 88, 0.1f));
FRAME_INFO(458, 31,  91, 89, 0.1f));
FRAME_INFO(567, 40, 103, 80, 0.1f));
FRAME_INFO(686, 32,  85, 88, 0.1f));
FRAME_INFO(792, 32,  86, 88, 0.1f));
FRAME_INFO(899, 31,  76, 89, 0.1f));
FRAME_INFO(993, 33,  92, 87, 0.1f));
*/