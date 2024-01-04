#include "pch.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "GameApp.h"
#include "Transform.h"
#include "GameApp.h"
#include "GameObject.h"
#include <d2d1.h>

AABB CameraComponent::m_CameraAABBbox = {};

CameraComponent::CameraComponent(GameObject* gameObject)
	: Transform(gameObject)
{
	m_transform = GetGameObject()->GetComponent<Transform>();
	m_CameraAABBbox.m_Extend.x = GameApp::m_hInstance->GetClientSize().width / 2.5f;
	m_CameraAABBbox.m_Extend.y = GameApp::m_hInstance->GetClientSize().height / 2.5f;

	posVector = m_transform->GetLocalPosition();
}

void CameraComponent::Update()
{
	InvertTransform();
	GetScreenTransform();

//	D2D1_MATRIX_3X2_F Transform = D2DRenderer::m_hInstance->m_CameraTransform * D2DRenderer::m_hInstance->m_ScreenTransform;
//	D2DRe nderer::m_hInstance->m_CameraTransform = Transform; 
	  
	m_CameraAABBbox.m_Center.x = m_transform->GetWorldLocation().x; // +m_CameraAABBbox.m_Extend.x;
	m_CameraAABBbox.m_Center.y = m_transform->GetWorldLocation().y; // +m_CameraAABBbox.m_Extend.y;


	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //왼쪽
		m_transform->SetLocalPosition({ posVector.x--, posVector.y });
		
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //오른쪽
		m_transform->SetLocalPosition({ posVector.x++, posVector.y });
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) { //위
		m_transform->SetLocalPosition({ posVector.x, posVector.y++ });
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //아래
		m_transform->SetLocalPosition({ posVector.x, posVector.y-- });
	}
}

void CameraComponent::Render(D2DRenderer* renderer)
{
	D2D1_MATRIX_3X2_F Transform = D2DRenderer::m_hInstance->m_ScreenTransform;
	D2DRenderer::m_hInstance->SetCameraTransform(Transform);
	renderer->SetTransform(Transform);
	renderer->DrawRectangle({
		-m_CameraAABBbox.m_Extend.x,
		-m_CameraAABBbox.m_Extend.y,
		m_CameraAABBbox.m_Extend.x,
		m_CameraAABBbox.m_Extend.y });
}

void CameraComponent::InvertTransform()	// Get 역행렬
{
	D2D1_MATRIX_3X2_F tmp = m_transform->GetWorldTransform();
	D2D1InvertMatrix(&tmp);
	D2DRenderer::m_hInstance->m_CameraTransform = tmp;
	//return tmp;
}

void CameraComponent::GetScreenTransform()	// Get 스크린 좌표계
{
	D2D1::Matrix3x2F screenTransform = D2D1::Matrix3x2F::Identity();
	screenTransform = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) 
		* D2D1::Matrix3x2F::Translation(GameApp::m_hInstance->GetClientSize().width / 2.f,
			GameApp::m_hInstance->GetClientSize().height / 2.f);
	D2DRenderer::m_hInstance->m_ScreenTransform = screenTransform;
	//return screenTransform;
}