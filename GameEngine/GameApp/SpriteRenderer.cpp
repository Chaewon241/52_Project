#include "pch.h"
#include "SpriteRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: RenderComponent(gameObject)
{
}

void SpriteRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	renderer->DrawSprite(m_Sprite);
}
