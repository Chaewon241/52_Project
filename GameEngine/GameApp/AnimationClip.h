#pragma once
#include <vector>
#include "json.hpp"

struct FRAME_INFO
{
	D2D1_RECT_F Source;			// �̹������� �ϳ��� ����� ��� ������ �ִ���
	float RenderTime;
	FRAME_INFO()
	{
		Source.left = 0;
		Source.top = 0;
		Source.right = 0;
		Source.bottom = 0;
		RenderTime = 0;
	}

	FRAME_INFO(float left, float top, float right, float bottom, float renderTime)
	{
		Source.left = left;
		Source.top = top;
		Source.right = right;
		Source.bottom = bottom;
		RenderTime = renderTime;
	}
	FRAME_INFO(D2D1_RECT_F rect, float time)
	{
		Source = rect;
		RenderTime = time;
	}
};

struct ANIMATION_INFO
{
	std::wstring m_Name; // �ִϸ��̼��� �̸�	�̸����� �˻��Ͽ� ����Ѵ�.
	std::vector<FRAME_INFO> m_Frames; // �������� ����
	ANIMATION_INFO()
		:m_Name(L"Default")
	{

	}
};

class AnimationClip
{
public:
	AnimationClip();
	AnimationClip(std::vector<ANIMATION_INFO> vec);
	void Save(const WCHAR* szFilePath);
	std::vector<ANIMATION_INFO> m_Animations;
};

