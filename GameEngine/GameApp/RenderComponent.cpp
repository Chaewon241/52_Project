#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "D2DRenderer.h"

RenderComponent::RenderComponent(GameObject* gameObject)
	:Component(gameObject)
{
	m_transform = GetGameObject()->GetComponent<Transform>();
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Update()
{
	m_transform = GetGameObject()->GetComponent<Transform>();
}

void RenderComponent::Render(D2DRenderer* renderer)
{
}