#pragma once
#include "Transform.h"

class GameObject;
class D2DRenderer;

class RectComponent : public Transform
{
private:
	Transform* m_Transform;
public:
	RectComponent(GameObject* go);
	~RectComponent();
public:
	void Update() override;
	void Render(D2DRenderer* renderer) override;
};

