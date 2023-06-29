#include "pch.h"
#include "AnimationComponent.h"
#include "AnimationAsset.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Transform.h"

AnimationComponent::AnimationComponent(GameObject* gameObject)
	: RenderComponent(gameObject)
	, m_pAnimationAsset(nullptr)
{
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::Update()
{
}

void AnimationComponent::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	if (m_pAnimationInstance)
	{
		m_pAnimationInstance->Render(pRenderTarget);
	}
}
