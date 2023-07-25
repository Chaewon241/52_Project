#include "pch.h"
#include "AnimationClip.h"


AnimationClip::AnimationClip()
{
}

AnimationClip::AnimationClip(std::vector<ANIMATION_INFO> vec)
{
	m_Animations = vec;
}

void AnimationClip::Save(const WCHAR* szFilePath)
{
	nlohmann::ordered_json obj;

	// todo : WString�� String���� �ٲ��ֱ�
	obj["m_BitmapFilePath"] = szFilePath;
}