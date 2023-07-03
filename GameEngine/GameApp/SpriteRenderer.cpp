#include "pch.h"
#include "SpriteRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: RenderComponent(gameObject)
	, width(80)
	, height(80)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	renderer->DrawAnimation()
}
