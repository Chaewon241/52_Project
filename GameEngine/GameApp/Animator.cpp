#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "D2DRenderer.h"

Animator::Animator(GameObject* gameObject)
	: Component(gameObject)
{
}

Animator::~Animator()
{
}

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
//		for (auto it : m_Listener)
//		{
//			it->OnAnimationEnd();
//		}
//
//		m_ProgressTime -= Frame.RenderTime;
//		m_FrameIndex = (m_FrameIndex + 1) % MaxFrameIndex;
//	}
//	// �����̿����� ������ ����
//	m_SrcRect = Frame.Source;
//	// �׸� ������ 0,0,with,height���� �����ϰ� ���� ��ġ�� Transform���� ����
//	m_DstRect = { 0,0,m_SrcRect.right - m_SrcRect.left,m_SrcRect.bottom - m_SrcRect.top };
//}

void Animator::Play(const std::string& name)
{
}

void Animator::SetAnimationAsset(const std::wstring& strAssetKey)
{
	assert(strAssetKey.empty() == false);
	m_AnimationKey = strAssetKey;
	m_AnimationClips = D2DRenderer::m_hInstance->CreateSharedAnimationAsset(strAssetKey);
}