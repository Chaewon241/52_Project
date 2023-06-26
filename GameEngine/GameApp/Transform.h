#pragma once

class GameObject;
class Component;

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
	void Update() override;
	void CalculateWorldTransform();
public:
	D2D1_VECTOR_2F GetLocalPosition() { return m_localPosition; }
	D2D1_VECTOR_2F GetLocalScale() { return m_localScale; }
	float GetLocalRotation() { return m_localRotation; }
	D2D1_MATRIX_3X2_F GetLocalTransform() { return m_localTransform; }

	D2D1_VECTOR_2F GetWorldPosition() { return m_worldPosition; }
	D2D1_VECTOR_2F GetWorldScale() { return m_worldScale; }
	float GetworldRotation() { return m_worldRotation; }
	D2D1_MATRIX_3X2_F GetWorldTransform() { return m_worldTransform; }
};