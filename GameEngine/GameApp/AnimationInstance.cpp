#include "pch.h"
#include "AnimationInstance.h"
#include "AnimationAsset.h"
#include "AnimationComponent.h"
#include "TimeManager.h"
#include "Object.h"

AnimationInstance::AnimationInstance()
	: Object()
	, m_pAnimationAsset(nullptr)
	, m_pAnimationComponent(nullptr)
	, m_AnimationIndex(0)
	, m_FrameIndex(0)
	, m_ProgressTime(0)
	, m_bMirror(0)
	, m_Speed(0)
{
}

AnimationInstance::~AnimationInstance()
{
}

void AnimationInstance::SetAnimationInfo(AnimationAsset* pAnimationInfo)
{
	m_pAnimationAsset = pAnimationInfo;
}

void AnimationInstance::Update()
{
	const ANIMATION_INFO& Animation = m_pAnimationAsset->m_Animations[m_AnimationIndex];
	const FRAME_INFO& Frame = Animation.m_Frames[m_FrameIndex];
	size_t MaxFrameIndex = Animation.m_Frames.size();

	m_ProgressTime += TimeManager::m_Instance->GetDeltatime() * m_Speed;


	while (Frame.RenderTime < m_ProgressTime)
	{
		m_ProgressTime -= Frame.RenderTime;
		m_FrameIndex = (m_FrameIndex + 1) % MaxFrameIndex;
	}
	// 이지미에서의 프레임 영역
	m_SrcRect = Frame.Source;
	// 그릴 영역을 0,0,with,height으로 설정하고 실제 위치는 Transform으로 설정
	m_DstRect = { 0,0,m_SrcRect.right - m_SrcRect.left,m_SrcRect.bottom - m_SrcRect.top };



	D2D1_VECTOR_2F Location;
	m_pAnimationComponent->GetWorldLocation(&Location);

	if (m_bMirror)
	{
		m_RenderTransform = D2D1::Matrix3x2F::Scale(-1.0f, -1.0f, D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(m_DstRect.right, 0.0f);
	}
	else
	{
		m_RenderTransform = D2D1::Matrix3x2F::Scale(1.0f, -1.0f, D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(0.0f, 0.0f);
	}
}

void AnimationInstance::Render(D2DRenderer* renderer)
{
	// 애니메이션렌더러도 만들어야하나?
	// 구조 : component -> TransformComponent
	//					-> RenderCompenent	-> EllipseComponent, RectComponent
	//										-> AnimationComponent
	// AnimationComponent는 AnimationInstance에 붙이는 느낌쓰? 붙이려면 AddComponent 사용하나요? 근데 이거 게임오브젝트에 있는데,,
}

void AnimationInstance::SetAnimationIndex(size_t index, bool Mirror)
{
}
