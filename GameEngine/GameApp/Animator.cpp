#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "D2DRenderer.h"

Animator* Animator::m_AnimatorInstance = nullptr;
AnimationClip* Animator::m_AnimationClips = nullptr;

Animator::Animator(GameObject* gameObject)
	: Component(gameObject)
{
}

Animator::~Animator()
{
}