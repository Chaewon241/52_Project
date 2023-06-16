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
	// ���丮�� ���ؼ� �����Ѵ�.
	D2DRenderer::m_Instance->ReleaseD2DBitmapFromFile(m_pBitmap);
}

/*
	�����͸� ������ ���ο� �����͸� �����°�
*/
void AnimationAsset::Build()
{
	D2DRenderer::m_Instance->CreateD2DBitmapFromFile(m_BitmapFilePath.c_str(), &m_pBitmap);
}

void AnimationAsset::SetBitmapFilePath(const WCHAR* szFilePath)
{
	m_BitmapFilePath = szFilePath;
}
