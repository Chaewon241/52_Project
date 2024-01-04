#pragma once
#include "Helper.h"
#include "NodeAnimation.h"

#include <assimp\anim.h>

// �� ��ü�� �ִϸ��̼� ����
class Animation
{
public:
	Animation() {};
	~Animation() {};

	void Create(aiAnimation* aiAnim);

public:
	std::vector<NodeAnimation> m_NodeAnims;
	double m_Duration;
	double m_TicksPerSecond;
};

