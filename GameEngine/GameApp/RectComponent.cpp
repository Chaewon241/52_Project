#include "pch.h"
#include "RectComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "D2DRenderer.h"

RectComponent::RectComponent(GameObject* go)
	:Transform(go)
{
}

RectComponent::~RectComponent()
{
}

void RectComponent::Update()
{
}

void RectComponent::Render(D2DRenderer* renderer)
{
	renderer->DrawRectangle(
		m_Transform->GetLocalPosition().x,
		m_Transform->GetLocalPosition().y,
		m_Transform->GetLocalPosition().x,
		m_Transform->GetLocalPosition().y
	);
}
