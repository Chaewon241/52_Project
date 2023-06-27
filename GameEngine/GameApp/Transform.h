#pragma once

#include "Component.h"

class GameObject;
class D2DRender;

class Transform : public Component
{
private:
	D2D1_VECTOR_2F m_localPosition;
	D2D1_VECTOR_2F m_localScale;
	float m_localRotation;
	D2D1_MATRIX_3X2_F m_localTransform;

	D2D1_VECTOR_2F m_worldPosition;
	D2D1_VECTOR_2F m_worldScale;
	float m_worldRotation;
	D2D1_MATRIX_3X2_F m_worldTransform;
public:
	Transform(GameObject* gameObject);
	~Transform();
public:
	virtual void Update() override;
	virtual void Render(D2DRenderer* renderer);
	void CalculateWorldTransform();
	void Rotate(float rotation);
public:
	void SetLocalPosition(D2D1_VECTOR_2F Position) 
	{ 
		m_localPosition = Position; 
		CalculateWorldTransform();
	}
	void SetLocalScale(D2D1_VECTOR_2F Scale)
	{
		m_localScale = Scale; 
		CalculateWorldTransform();
	}
	void SetLocalRotation(float rotation) 
	{	
		m_localRotation = rotation;
		CalculateWorldTransform();
	}

	D2D1_VECTOR_2F GetLocalPosition() { return m_localPosition; }
	D2D1_VECTOR_2F GetLocalScale() { return m_localScale; }
	float GetLocalRotation() { return m_localRotation; }
	D2D1_MATRIX_3X2_F GetLocalTransform() { return m_localTransform; }

	D2D1_VECTOR_2F GetWorldPosition() { return m_worldPosition; }
	D2D1_VECTOR_2F GetWorldScale() { return m_worldScale; }
	float GetworldRotation() { return m_worldRotation; }
	D2D1_MATRIX_3X2_F GetWorldTransform() { return m_worldTransform; }
};