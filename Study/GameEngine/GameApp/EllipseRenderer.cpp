#include "pch.h"
#include "EllipseRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"

EllipseRenderer::EllipseRenderer(GameObject* gameObject)
	:RenderComponent(gameObject)
	, width(80)
	, height(80)
{
}

EllipseRenderer::~EllipseRenderer()
{
}

void EllipseRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	renderer->DrawEllipse(
		{ width / 2.f, height / 2.f },
		50, 50
	);
}
