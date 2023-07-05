#pragma once
#include <vector>

struct FRAME_INFO
{
	D2D1_RECT_F Source;			// �̹������� �ϳ��� ����� ��� ������ �ִ���

	FRAME_INFO()
	{
		Source.left = 0;
		Source.top = 0;
		Source.right = 0;
		Source.bottom = 0;
	}

	FRAME_INFO(float left, float top, float right, float bottom)
	{
		Source.left = left;
		Source.top = top;
		Source.right = right;
		Source.bottom = bottom;
	}
	FRAME_INFO(D2D1_RECT_F rect, float time)
	{
		Source = rect;
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
	std::vector<ANIMATION_INFO> m_Animations;
};

