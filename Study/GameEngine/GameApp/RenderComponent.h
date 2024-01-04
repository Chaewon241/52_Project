#pragma once
#include "Transform.h"
#include "AABB.h"

class D2DRenderer;
class Transform;
class Sprite;
class AnimationClip;
class CameraComponent;

class RenderComponent : public Transform
{
protected:
	AnimationClip* m_AnimationClip;
	Sprite* m_Sprite;
	AABB m_BoundingBox;
	bool m_IsAABB;
public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();
public:
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};