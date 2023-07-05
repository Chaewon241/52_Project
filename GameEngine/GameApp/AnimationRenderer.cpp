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
	renderer->SetTransform(m_transform->GetWorldTransform());
	//m_AnimationClip->m_Animations;
	//animationInfo.m_Name	

	if (frameIndex != -1)
	{
		// ken.png 비트맵
		renderer->DrawAnimation(m_AnimationClip, m_anmationPath, m_curFrame[frameIndex]);
	}
}

// setstate 해서 상태 불러오기
