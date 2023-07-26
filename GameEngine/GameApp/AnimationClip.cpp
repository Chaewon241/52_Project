#include "pch.h"
#include "AnimationClip.h"
#include "json.hpp"
#include "D2DRenderer.h"
#include <fstream>

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
	
	for (int i = 0; i < m_Animations.size(); i++)
	{
		ANIMATION_INFO& animation = m_Animations[i];
		nlohmann::ordered_json animationObj;
		obj["animationName"] = animation.m_Name;

		for (int j = 0; j < animation.m_Frames.size(); j++)
		{
			FRAME_INFO& frame = animation.m_Frames[j];
			nlohmann::ordered_json frameObj;
			frameObj["Source.left"] = frame.Source.left;
			frameObj["Source.top"] = frame.Source.top;
			frameObj["Source.right"] = frame.Source.right;
			frameObj["Source.bottom"] = frame.Source.bottom;
			frameObj["RenderTime"] = (frame.RenderTime);
			animationObj["m_Frames"].push_back(frameObj);
		}
		obj["m_Animations"].push_back(animationObj);
	}
	std::ofstream ofs(szFilePath);
	ofs << obj.dump(2);
	ofs.close();
}

bool AnimationClip::Load(const WCHAR* szFilePath, D2DRenderer renderer)
{
	std::ifstream ifs(szFilePath, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	nlohmann::ordered_json obj;
	ifs >> obj;
	ifs.close();

	for (auto& animationObj : obj["m_Animations"])
	{
		ANIMATION_INFO animation;
		animation.m_Name = animationObj["m_Name"].get<std::wstring>();
		for (auto& frameObj : animationObj["m_Frames"])
		{
			FRAME_INFO frame;
			frame.Source.left = frameObj["Source.left"].get<float>();
			frame.Source.top = frameObj["Source.top"].get<float>();
			frame.Source.right = frameObj["Source.right"].get<float>();
			frame.Source.bottom = frameObj["Source.bottom"].get<float>();
			frame.RenderTime = frameObj["RenderTime"].get<float>();
			animation.m_Frames.push_back(frame);
		}
		m_Animations.push_back(animation);
	}
	return true;
}
