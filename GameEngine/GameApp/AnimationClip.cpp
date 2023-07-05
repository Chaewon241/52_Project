#include "pch.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(std::wstring sheetName, std::vector<std::vector<int>> vec)
{
	m_ClipName = sheetName;
	m_vClip = vec;
}
