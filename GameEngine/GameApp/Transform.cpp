#include "pch.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component.h"

Transform::Transform(GameObject* gameObject)
	: Component(gameObject)
	, m_localPosition({ 0, 0 })
	, m_localScale({ 0, 0 })
	, m_localRotation(0.f)
	, m_worldPosition({ 0, 0 })
	, m_worldScale({ 0, 0 })
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
}
