#include "pch.h"
#include "NodeAnimation.h"

void NodeAnimation::Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scaling)
{
	durationTime += progressTime;

	if (frameIndex + 1 >= m_AnimationKey.size())
	{
		frameIndex = 0;
		durationTime = 0;
	}

	const AnimationKey& currentKey = m_AnimationKey[frameIndex];
	const AnimationKey& nextKey = m_AnimationKey[frameIndex + 1];

	if (durationTime > nextKey.m_Time)
	{
		frameIndex++;
	}

	float factor = (durationTime - currentKey.m_Time) / (nextKey.m_Time - currentKey.m_Time);

	// 선형 보간 적용
	position = Vector3::Lerp(currentKey.m_Position, nextKey.m_Position, factor);
	rotation = SphericalInterpolation(currentKey.m_Rotation, nextKey.m_Rotation, factor);
	scaling = Vector3::Lerp(currentKey.m_Scaling, nextKey.m_Scaling, factor);
}

Quaternion NodeAnimation::SphericalInterpolation(const Quaternion& start, const Quaternion& end, float factor)
{
	Quaternion result;

	Quaternion::Slerp(start, end, factor, result);

	result.Normalize();

	return result;
}
