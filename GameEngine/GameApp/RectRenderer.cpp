#include "pch.h"
#include "RectRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"

RectRenderer::RectRenderer(GameObject* gameObject)
	:RenderComponent(gameObject)
	, m_Rect({ 0 })
{
}

RectRenderer::~RectRenderer()
{
}

void RectRenderer::SetExtend(float x, float y)
{
	m_Rect.right = x;
	m_Rect.bottom = y;
	m_Rect.left = -x;
	m_Rect.top = -y;

	m_BoundingBox.m_Extend.x = x;
	m_BoundingBox.m_Extend.y = y;
}

void RectRenderer::Render(D2DRenderer* renderer)
{
	D2D1_MATRIX_3X2_F Transform = m_transform->GetWorldTransform() * D2DRenderer::m_CameraTransform * D2DRenderer::m_ScreenTransform;
	renderer->SetTransform(Transform);

	renderer->DrawRectangle(m_Rect);
}

