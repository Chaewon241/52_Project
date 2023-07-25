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

	// todo : WString을 String으로 바꿔주기
	obj["m_BitmapFilePath"] = szFilePath;
}