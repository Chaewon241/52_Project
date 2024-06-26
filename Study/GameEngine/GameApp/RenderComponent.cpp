#include "pch.h"
#include "Transform.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "D2DRenderer.h"
#include "AnimationClip.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(GameObject* gameObject)
	:Transform(gameObject)
	, m_IsAABB(false)
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

	m_BoundingBox.m_Center = m_transform->GetWorldLocation();

	m_IsAABB = m_BoundingBox.CheckIntersect(CameraComponent::m_CameraAABBbox);
}

void RenderComponent::Render(D2DRenderer* renderer)
{
}

