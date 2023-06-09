#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "D2DRenderer.h"
#include "AnimationClip.h"
#include "SpriteRenderer.h"
#include "Sprite.h"

RenderComponent::RenderComponent(GameObject* gameObject)
	:Component(gameObject)
{
	m_transform = GetGameObject()->GetComponent<Transform>();
	m_Sprite = GetGameObject()->GetComponent<Sprite>();
	m_AnimationClip = GetGameObject()->GetComponent<AnimationClip>();
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Update()
{
	m_transform = GetGameObject()->GetComponent<Transform>();
	m_Sprite = GetGameObject()->GetComponent<Sprite>();
	m_AnimationClip = GetGameObject()->GetComponent<AnimationClip>();
}

void RenderComponent::Render(D2DRenderer* renderer)
{
}