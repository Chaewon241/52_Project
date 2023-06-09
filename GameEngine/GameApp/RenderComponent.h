#pragma once
#include "Component.h"

class GameObject;
class D2DRenderer;
class Transform;
class Sprite;
class AnimationClip;

class RenderComponent : public Component
{
protected:
	Transform* m_transform;
	AnimationClip* m_AnimationClip;
	Sprite* m_Sprite;
public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();
public:
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};