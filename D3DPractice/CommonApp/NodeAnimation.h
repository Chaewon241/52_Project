#pragma once
#include "Helper.h"

struct AnimationKey
{
	double m_Time;
	Vector3 m_Position;
	Quaternion m_Rotation;
	Vector3 m_Scaling;
};

// ���� ����� �ִϸ��̼� ����
class NodeAnimation
{
public:
	string m_NodeName;
	vector<AnimationKey> m_AnimationKey;

	int frameIndex = 0;
	float durationTime = 0;

public:
	void Evaluate(
		float progressTime,
		Vector3& position,
		Quaternion& rotation,
		Vector3& scaling
	);

	// ���� ���� �Լ�
	Quaternion SphericalInterpolation(const Quaternion& start, const Quaternion& end, float factor);
};

