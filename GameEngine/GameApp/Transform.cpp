#include "pch.h"
#include "Transform.h"
#include "RectObject.h"
#include "Component.h"

Transform::Transform(RectObject* rectObject)
	: m_localPosition({ 0, 0 })
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

Transform* Transform::GetParent() const
{
	return m_parent;
}

void Transform::SetParent(Transform* parent)
{
	m_parent = parent;
}

void Transform::CalculateWorldTransform()
{
	m_localTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_localScale.x, m_localScale.y)) *
		D2D1::Matrix3x2F::Rotation(m_localRotation) *
		D2D1::Matrix3x2F::Translation(m_localPosition.x, m_localPosition.y);

	if (m_parent != nullptr)
	{
		m_worldTransform = m_localTransform * m_parent->m_worldTransform;
	}
}
