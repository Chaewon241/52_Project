#include "pch.h"
#include "CameraComponent.h"
#include "GameApp.h"
#include "Transform.h"

CameraComponent::CameraComponent(GameObject* gameObject)
	: Transform(gameObject)
{
	m_BoundingBox.m_Extend.x = GameApp::m_hInstance->GetClientSize().width / 1.5f;
	m_BoundingBox.m_Extend.y = GameApp::m_hInstance->GetClientSize().height / 2.f;
}

void CameraComponent::Update()
{
	m_BoundingBox.m_Center.x = GetWorldLocation().x + m_BoundingBox.m_Extend.x;
	m_BoundingBox.m_Center.y = GetWorldLocation().y + m_BoundingBox.m_Extend.y;
}

void CameraComponent::Render(D2DRenderer* renderer)
{
	D2D1_MATRIX_3X2_F Transform = GetLocalTransform() * GetWorldTransform() * D2DRenderer::m_CameraTransform * D2DRenderer::m_ScreenTransform;
	renderer->SetTransform(Transform);
	renderer->DrawRectangle({ -m_BoundingBox.m_Extend.x, -m_BoundingBox.m_Extend.y, m_BoundingBox.m_Extend.x, m_BoundingBox.m_Extend.y });
}