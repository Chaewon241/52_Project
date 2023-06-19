#include "pch.h"
#include "AnimationInstance.h"
#include "AnimationAsset.h"
#include "D2DRenderer.h"

AnimationInstance::AnimationInstance()
	:m_pAnimationAsset(nullptr), m_AnimationIndex(0), m_FrameIndex(0), m_ProgressTime(0), m_bMirror(false), m_Speed(1.0f)
{
	m_SrcRect = { 0.0f,0.0f,0.0f,0.0f };
	m_DstRect = { 0.0f,0.0f,0.0f,0.0f };
	m_Position = { 0.0f,0.0f };
	m_Transform = D2D1::Matrix3x2F::Identity();

}

AnimationInstance::~AnimationInstance()
{
	//	D2DRenderer::m_Instance->ReleaseAnimationAsset(m_pAnimationAsset);
}

void AnimationInstance::SetAnimationInfo(AnimationAsset* pAnimationInfo)
{
	m_pAnimationAsset = pAnimationInfo;
	pAnimationInfo->AddRef();
}

void AnimationInstance::Update(float deltaTime)
{
	m_ProgressTime += deltaTime * m_Speed;

	if (m_ProgressTime >= m_pAnimationAsset->m_Animations[m_AnimationIndex][m_FrameIndex].RenderTime)
	{
		m_FrameIndex++;
	}

}

void AnimationInstance::Render(ID2D1RenderTarget* pRenderTarget)
{
	pRenderTarget->SetTransform(m_Transform);
	pRenderTarget->DrawBitmap(m_pAnimationAsset->m_pBitmap, m_DstRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, m_SrcRect);
}

void AnimationInstance::SetAnimationIndex(size_t index, bool bMirror)
{
	m_AnimationIndex = index;
	m_FrameIndex = 0;
	m_ProgressTime = 0.0f;
	m_bMirror = bMirror;
}