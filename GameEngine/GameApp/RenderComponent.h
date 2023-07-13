#pragma once
#include "Transform.h"

class D2DRenderer;
class Transform;
class Sprite;
class AnimationClip;

class RenderComponent : public Transform
{
protected:
	AnimationClip* m_AnimationClip;
	Sprite* m_Sprite;
public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();
public:
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};