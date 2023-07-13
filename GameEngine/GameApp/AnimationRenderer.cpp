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
	float deltaTime = TimeManager::m_Instance->GetDeltatime();
	
	if (m_time > m_curFrame[frameIndex].RenderTime)
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
	//m_
	if (frameIndex != -1)
	{
		renderer->DrawAnimation(m_AnimationClip, m_anmationPath, m_curFrame[frameIndex]);
	}
}

// setstate �ؼ� ���� �ҷ�����
