#pragma once
#include <vector>

struct Frame_Info
{
	D2D1_RECT_F SourceRect;
	float		RenderTime;

	Frame_Info(float x, float y, float width, float height, float time)
	{
		SourceRect.left = x;
		SourceRect.top = y;
		SourceRect.right = x + width;
		SourceRect.bottom = y + height;
		RenderTime = time;
	}
	Frame_Info(D2D1_RECT_F rect, float time)
	{
		SourceRect = rect;
		RenderTime = time;
	}
};

class AnimationClip
{
private:
	std::vector<Frame_Info> m_animationClips;
public:
	AnimationClip(float left, float top, float width, float height, float length)
	{
	}
	~AnimationClip()
	{
	}
};