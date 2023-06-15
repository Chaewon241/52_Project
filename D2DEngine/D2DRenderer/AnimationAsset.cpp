#include "pch.h"
#include "AnimationAsset.h"
#include "D2DRenderer.h"
#include "GameApp.h"

AnimationAsset::AnimationAsset()
	:m_pBitmap(nullptr)
{

}

AnimationAsset::~AnimationAsset()
{
	// 팩토리를 통해서 해제한다.
	D2DRenderer::m_Instance->ReleaseD2DBitmapFromFile(m_pBitmap);
}

/*
	데이터를 가지고 새로운 데이터를 만들어내는것
*/
void AnimationAsset::Build()
{
	D2DRenderer::m_Instance->CreateD2DBitmapFromFile(m_BitmapFilePath.c_str(), &m_pBitmap);
}

void AnimationAsset::SetBitmapFilePath(const WCHAR* szFilePath)
{
	m_BitmapFilePath = szFilePath;
}

