#include "pch.h"
#include "AnimationRenderer.h"
#include "D2DRenderer.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "TimeManager.h"
#include "AnimationClip.h"


void AnimationRenderer::Update()
{
	static float m_time = 0.f;
	float deltaTime = TimeManager::m_Instance->GetDeltatime();
	static int frameIndex = 0;

	if (m_time > 0.2f)
	{
		aniVec = m_AnimationClip->m_vClip[frameIndex % m_AnimationClip->m_vClip.size()];
		frameIndex++;
		m_time -= 0.2f;
	}
	m_time += deltaTime;
}

void AnimationRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	if (aniVec.size() != 0)
	{
		renderer->DrawAnimation(m_AnimationClip, aniVec);
	}
}

AnimationRenderer::AnimationRenderer(GameObject* gameObject)
	: RenderComponent(gameObject)
{
}
