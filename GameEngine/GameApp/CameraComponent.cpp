#include "pch.h"
#include "CameraComponent.h"
#include "GameApp.h"
#include "Transform.h"

CameraComponent::CameraComponent(GameObject* gameObject)
	: RenderComponent(gameObject)
{
	m_BoundingBox.m_Extend.x = GameApp::m_hInstance->GetClientSize().width / 2.f;
	m_BoundingBox.m_Extend.y = GameApp::m_hInstance->GetClientSize().height / 2.f;
}

void CameraComponent::Update()
{
	__super::Update();

	m_BoundingBox.m_Center.x = m_transform->GetWorldLocation().x + m_BoundingBox.m_Extend.x;
	m_BoundingBox.m_Center.y = m_transform->GetWorldLocation().y + m_BoundingBox.m_Extend.y;
}
