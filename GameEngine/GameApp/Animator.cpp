#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "D2DRenderer.h"

Animator* Animator::m_AnimatorInstance = nullptr;
AnimationClip* Animator::m_AnimationClips = nullptr;

Animator::Animator(GameObject* gameObject)
	: Component(gameObject)
{
}

Animator::~Animator()
{
}
//
//void Animator::Update()
//{
//	const FRAME_INFO& Frame = m_pAnimationInfo->m_Frames[m_FrameIndex];
//	size_t MaxFrameIndex = m_pAnimationInfo->m_Frames.size();
//
//	m_ProgressTime += GameApp::m_deltaTime * m_Speed;
//
//
//	while (Frame.RenderTime < m_ProgressTime)
//	{
//		
//
//		m_ProgressTime -= Frame.RenderTime;
//		m_FrameIndex = (m_FrameIndex + 1) % MaxFrameIndex;
//	}
//	m_SrcRect = Frame.Source;
//	m_DstRect = { 0,0,m_SrcRect.right - m_SrcRect.left,m_SrcRect.bottom - m_SrcRect.top };
//}



void Animator::Play(const std::string& name)
{
}
