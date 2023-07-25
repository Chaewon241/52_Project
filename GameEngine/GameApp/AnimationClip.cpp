#include "pch.h"
#include "AnimationClip.h"


AnimationClip::AnimationClip()
{
}

AnimationClip::AnimationClip(std::vector<ANIMATION_INFO> vec)
{
	m_Animations = vec;
}
