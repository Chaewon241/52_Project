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
	static int frameIndex = 0;

	if (m_time > 0.2f)
	{
		m_frame = frameIndex % m_curFrame.size();
		frameIndex++;
		m_time -= 0.2f;
	}
	m_time += deltaTime;
}

void AnimationRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());
	//m_AnimationClip->m_Animations;
	//animationInfo.m_Name	

	if (m_frame != -1)
	{
		// ken.png 비트맵
		renderer->DrawAnimation(m_AnimationClip, m_anmationPath, m_curFrame[m_frame]);
	}
}

// setstate 해서 상태 불러오기
