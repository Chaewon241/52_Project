#include "pch.h"
#include "SpriteRenderer.h"
#include "RenderComponent.h"
#include "D2DRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: RenderComponent(gameObject)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Render(D2DRenderer* renderer)
{
	renderer->SetTransform(m_transform->GetWorldTransform());

	renderer->DrawAnimation(m_Animator->m_AnimationClips);
}
