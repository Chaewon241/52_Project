#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"

Animator::Animator(GameObject* gameObject)
	: Component(gameObject)
{
}

Animator::~Animator()
{
}

void Animator::Play(const std::string& name)
{
}
