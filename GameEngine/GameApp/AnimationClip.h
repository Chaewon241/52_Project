#pragma once
#include <vector>

class AnimationClip
{
public:
	AnimationClip(std::wstring sheetName, std::vector<std::vector<int>> vec);
	std::wstring m_ClipName;
	std::vector<std::vector<int>> m_vClip;
};

