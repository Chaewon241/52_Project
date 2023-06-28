#pragma once
#include "Component.h"

class GameObject;
class D2DRenderer;
class Transform;

class RenderComponent : public Component
{
protected:
	Transform* m_transform;
public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();
public:
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};