#include "pch.h"
#include "RectRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"

RectRenderer::RectRenderer(GameObject* gameObject)
	:RenderComponent(gameObject)
	, width(80)
	, height(80)
{
}

RectRenderer::~RectRenderer()
{
}

void RectRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	renderer->DrawRectangle(
		-(width / 2.f),
		-(height / 2.f),
		width / 2.f,
		height / 2.f
	);
}
