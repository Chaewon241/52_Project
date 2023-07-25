#pragma once
#include "AABB.h"
#include "Component.h"

class GameObject;
class D2DRender;
class TimeManager;

class Transform : public Component
{
public:
	Transform(GameObject* gameObject);
	~Transform();
private:
	D2D1_VECTOR_2F m_localPosition;
	D2D1_VECTOR_2F m_localScale;
	D2D1_VECTOR_2F m_worldScale;
	float m_localRotation;
	D2D1_MATRIX_3X2_F m_localTransform;
	D2D1_MATRIX_3X2_F m_worldTransform;
protected:
	Transform* m_transform;

public:
	virtual void Update() override;
	virtual void Render(D2DRenderer* renderer);
	void CalculateWorldTransform();
	void CalculateWorldScale();
	void Rotate(float rotation);
	virtual void SerializeOut(nlohmann::ordered_json& object) override;
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
		CalculateWorldScale();
	}
	void SetLocalRotation(float rotation) 
	{	
		m_localRotation = rotation;
		CalculateWorldTransform();
	}

	D2D1_VECTOR_2F GetLocalPosition() { return m_localPosition; }
	D2D1_VECTOR_2F GetLocalScale() { return m_localScale; }
	D2D1_VECTOR_2F GetWorldScale() { return m_worldScale; }
	float GetLocalRotation() { return m_localRotation; }
	D2D1_MATRIX_3X2_F GetLocalTransform() { return m_localTransform; }

	D2D1_MATRIX_3X2_F GetWorldTransform() { return m_worldTransform; }

	D2D1_VECTOR_2F GetWorldLocation()
	{
		D2D1_VECTOR_2F out;
		out.x = m_worldTransform._31;
		out.y = m_worldTransform._32;
		return out;
	}
};