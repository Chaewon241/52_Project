#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timemanager.h"
#include "D2DRenderer.h"

Transform::Transform(GameObject* gameObject)
	: Component(gameObject)
	, m_localPosition({ 0.f, 0.f })
	, m_localScale({ 1.f, 1.f })
	, m_localRotation(0.f)
	, m_worldPosition({ 0.f, 0.f })
	, m_worldScale({ 1.f, 1.f })
	, m_worldRotation(0.f)
	, m_localTransform(D2D1::Matrix3x2F::Identity())
	, m_worldTransform(m_localTransform)
{
}

Transform::~Transform()
{
}

void Transform::Update()
{
	static float time = 0.f;
	TimeManager tm;
	float deltaTime = tm.GetDeltatime();

	if (time > 0.001f)
	{
		time -= 0.001f;
	}
	time += deltaTime;
}

void Transform::Render(D2DRenderer* renderer)
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
