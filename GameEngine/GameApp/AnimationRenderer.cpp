#include "pch.h"
#include "AnimationRenderer.h"
#include "D2DRenderer.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "TimeManager.h"

AnimationRenderer::AnimationRenderer(GameObject* gameObject)
	: RenderComponent(gameObject)
{
}

void AnimationRenderer::Update()
{

	RenderComponent::Update();

	float deltaTime = TimeManager::m_Instance->GetDeltatime();
	
	if (m_time > m_curFrame[frameIndex].RenderTime)
	{
		frameIndex++;
		frameIndex %= m_curFrame.size();
		m_time = 0.f;
	}
	m_time += deltaTime;
	m_AniRect = { 
		m_curFrame[frameIndex].Source.right - m_curFrame[frameIndex].Source.left,
		m_curFrame[frameIndex].Source.top - m_curFrame[frameIndex].Source.bottom };
}

void AnimationRenderer::Render(D2DRenderer* renderer)
{
	if (m_IsAABB)
	{
		return;
	}
	D2D1_MATRIX_3X2_F Transform = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * m_transform->GetWorldTransform()
		* D2DRenderer::m_hInstance->m_CameraTransform 
		* D2DRenderer::m_hInstance->m_ScreenTransform;
	renderer->SetTransform(Transform);
	//m_AnimationClip->m_Animations;
	//animationInfo.m_Name
	//m_

	if (frameIndex != -1)
	{
		renderer->DrawAnimation(m_anmationPath, m_curFrame[frameIndex]);
		renderer->DrawRectangle(m_AniRect);
	}
}

// setstate 해서 상태 불러오기
