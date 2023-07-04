#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "D2DRenderer.h"
#include "AnimationClip.h"
#include "SpriteRenderer.h"
#include "Animator.h"

RenderComponent::RenderComponent(GameObject* gameObject)
	:Component(gameObject)
{
	m_transform = GetGameObject()->GetComponent<Transform>();
	m_Animator = GetGameObject()->GetComponent<Animator>();
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Update()
{
}

void RenderComponent::Render(D2DRenderer* renderer)
{
}