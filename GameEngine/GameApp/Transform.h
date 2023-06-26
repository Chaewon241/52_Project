#pragma once

class RectObject;

class Transform : public Component
{
public:
	D2D1_VECTOR_2F m_localPosition;
	D2D1_VECTOR_2F m_localScale;
	float m_localRotation;
	D2D1_MATRIX_3X2_F m_localTransform;

	D2D1_VECTOR_2F m_worldPosition;
	D2D1_VECTOR_2F m_worldScale;
	float m_worldRotation;
	D2D1_MATRIX_3X2_F m_worldTransform;

	//¶Ç ½á?
	Transform* m_parent;

public:
	Transform(RectObject* rectObject);
	~Transform();

	void Update() override;

	Transform* GetParent() const;
	void SetParent(Transform* parent);

	D2D1_VECTOR_2F GetLocalPosition() { return m_localPosition; }
	D2D1_VECTOR_2F GetLocalScale() { return m_localScale; }
	float GetLocalRatation() { return m_localRotation; }

	D2D1_VECTOR_2F GetWorldPosition() { return m_worldPosition; }
	D2D1_VECTOR_2F GetWorldScale() { return m_worldScale; }
	float GetworldRotation() { return m_worldRotation; }

	void CalculateWorldTransform();
};