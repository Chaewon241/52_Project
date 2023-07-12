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
	static float m_time = 0.f;
	float deltaTime = TimeManager::m_Instance->GetDeltatime();

	if (m_time > 0.03f)
	{
		frameIndex++;
		frameIndex %= m_curFrame.size();
		m_time = 0.f;
	}
	m_time += deltaTime;
}

void AnimationRenderer::Render(D2DRenderer* renderer)
{
	DrawWorldTransform(renderer);
	D2D1_MATRIX_3X2_F Transform = m_transform->GetWorldTransform() * D2DRenderer::m_CameraTransform;
	renderer->SetTransform(Transform);
	//m_AnimationClip->m_Animations;
	//animationInfo.m_Name	

	if (frameIndex != -1)
	{
		renderer->DrawAnimation(m_AnimationClip, m_anmationPath, m_curFrame[frameIndex]);
	}
	renderer->DrawCrossLine();
}


void AnimationRenderer::DrawWorldTransform(D2DRenderer* renderer)
{
	D2D1_MATRIX_3X2_F Transform = m_transform->GetWorldTransform() * D2DRenderer::m_CameraTransform * D2DRenderer::m_ScreenTransform;
	renderer->SetTransform(Transform);
	renderer->DrawCrossLine();
}

// setstate 해서 상태 불러오기
