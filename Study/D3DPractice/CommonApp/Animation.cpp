#include "pch.h"
#include "Animation.h"

void Animation::Create(aiAnimation* aiAnim)
{
	m_Duration = aiAnim->mDuration;
	m_TicksPerSecond = aiAnim->mTicksPerSecond;

	// �ִϸ��̼ǿ� ������ aiNode�� ��
	m_NodeAnims.resize(aiAnim->mNumChannels);

	for (int i = 0; i < aiAnim->mNumChannels; i++)
	{
		m_NodeAnims[i].m_NodeName = aiAnim->mChannels[i]->mNodeName.C_Str();

		// mPositionKeys, mRotationKeys, mScalingKeys ������ �Ȱ��Ƽ�
		// mNumPositionKeys �ε��� ó����.
		for (int j = 0; j < aiAnim->mChannels[i]->mNumPositionKeys; j++)
		{
			AnimationKey animKey;

			Vector3 position = Vector3(
				aiAnim->mChannels[i]->mPositionKeys[j].mValue.x,
				aiAnim->mChannels[i]->mPositionKeys[j].mValue.y,
				aiAnim->mChannels[i]->mPositionKeys[j].mValue.z
			);
			Quaternion rotation = Quaternion(
				aiAnim->mChannels[i]->mRotationKeys[j].mValue.x,
				aiAnim->mChannels[i]->mRotationKeys[j].mValue.y,
				aiAnim->mChannels[i]->mRotationKeys[j].mValue.z,
				aiAnim->mChannels[i]->mRotationKeys[j].mValue.w	// ȸ�� ����
			);
			Vector3 scaling = Vector3(
				aiAnim->mChannels[i]->mScalingKeys[j].mValue.x,
				aiAnim->mChannels[i]->mScalingKeys[j].mValue.y,
				aiAnim->mChannels[i]->mScalingKeys[j].mValue.z
			);

			// �ִϸ��̼� ��ü �� �ð�
			animKey.m_Time = aiAnim->mChannels[i]->mPositionKeys[j].mTime / aiAnim->mTicksPerSecond;
			animKey.m_Rotation = rotation;
			animKey.m_Position = position;
			animKey.m_Scaling = scaling;

			m_NodeAnims[i].m_AnimationKey.push_back(animKey);
		}
	}
}
