#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeManager.h"
#include "D2DRenderer.h"

Transform::Transform(GameObject* gameObject)
	: Component(gameObject)
	, m_localPosition({ 0.f, 0.f })
	, m_localScale({ 1.f, 1.f })
	, m_localRotation(0.f)
	, m_localTransform(D2D1::Matrix3x2F::Identity())
	, m_worldTransform(m_localTransform)
	, m_transform(0)
{
}

Transform::~Transform()
{
}

void Transform::Update()
{
	static float time = 0.f;
	float deltaTime = TimeManager::m_Instance->GetDeltatime();

	if (time > 0.001f)
	{
		//Rotate(0.5f);
		time -= 0.001f;
	}
	time += deltaTime;

	CalculateWorldTransform();
	CalculateWorldScale();
}

void Transform::Render(D2DRenderer* renderer)
{
}

void Transform::Rotate(float rotation)
{
	m_localRotation += rotation;

	if (m_localRotation > 360.0f)
		m_localRotation -= 360.0f;

}

void Transform::SerializeOut(nlohmann::ordered_json& object)
{
}

void Transform::CalculateWorldTransform()
{
	GameObject* _parent = m_GameObject->GetParent();

	m_localTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_localScale.x, m_localScale.y)) *
		D2D1::Matrix3x2F::Rotation(m_localRotation) *
		D2D1::Matrix3x2F::Translation(m_localPosition.x, m_localPosition.y);

	if (_parent != nullptr)
	{
		m_worldTransform = m_localTransform * _parent->GetComponent<Transform>()->GetWorldTransform();
	}
	else
	{
		m_worldTransform = m_localTransform;
	}
}

void Transform::CalculateWorldScale()
{
	GameObject* _parent = m_GameObject->GetParent();

	if (_parent != nullptr)
	{
		m_worldScale.x = m_localScale.x * _parent->GetComponent<Transform>()->GetWorldScale().x;
		m_worldScale.y = m_localScale.y * _parent->GetComponent<Transform>()->GetWorldScale().y;
	}
	else
	{
		m_worldScale = m_localScale;
	}
}