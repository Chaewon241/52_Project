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
	void Render(D2DRenderer* renderer) override;
	void CalculateWorldTransform();
public:
	void SetLocalPosition(D2D1_VECTOR_2F Position) { m_localPosition = Position; }
	void SetLocalScale(D2D1_VECTOR_2F Scale) { m_localScale = Scale; }
	void SetLocalRotation(float rotation) {	m_localRotation = rotation;	}

	D2D1_VECTOR_2F GetLocalPosition() { return m_localPosition; }
	D2D1_VECTOR_2F GetLocalScale() { return m_localScale; }
	float GetLocalRotation() { return m_localRotation; }
	D2D1_MATRIX_3X2_F GetLocalTransform() { return m_localTransform; }

	D2D1_VECTOR_2F GetWorldPosition() { return m_worldPosition; }
	D2D1_VECTOR_2F GetWorldScale() { return m_worldScale; }
	float GetworldRotation() { return m_worldRotation; }
	D2D1_MATRIX_3X2_F GetWorldTransform() { return m_worldTransform; }
};