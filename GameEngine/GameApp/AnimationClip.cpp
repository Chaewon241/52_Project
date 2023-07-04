#include "pch.h"
#include "AnimationClip.h"
#include "D2DRenderer.h"
#include <cassert>

void AnimationClip::SetD2DBitmap(const WCHAR* szFilePath)
{
	m_BitmapFilePath = szFilePath;
	//HRESULT hr = D2DRenderer::m_hInstance->CreateSharedD2DBitmapFromFile(m_BitmapFilePath, &m_pBitmap);
}
