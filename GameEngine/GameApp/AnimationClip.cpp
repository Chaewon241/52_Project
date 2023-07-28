#include "pch.h"
#include "AnimationClip.h"
#include "json.hpp"
#include "D2DRenderer.h"
#include <fstream>
#include <codecvt>

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
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	obj["m_BitmapFilePath"] = converter.to_bytes(szFilePath);
	
	for (int i = 0; i < m_Animations.size(); i++)
	{
		ANIMATION_INFO& animation = m_Animations[i];
		nlohmann::ordered_json animationObj;
		animationObj["Name"] = converter.to_bytes(animation.m_Name);

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

bool AnimationClip::Load(const WCHAR* szFilePath)
{
	std::ifstream ifs(szFilePath, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	nlohmann::ordered_json obj = nlohmann::ordered_json::parse(str);

	for (auto& animationObj : obj["m_Animations"])
	{
		ANIMATION_INFO animation;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		animation.m_Name = converter.from_bytes(animationObj["Name"].get<std::string>());
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
