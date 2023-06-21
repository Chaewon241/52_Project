#include "pch.h"
#include "SceneComponent.h"

void SceneComponent::Update()
{

	UpdateTransform();

	for (auto& pChild : m_Children)
	{
		pChild
	}
}

void SceneComponent::Render()
{
}

void SceneComponent::UpdateTransform()
{
	m_RelativeTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_RelativeScale.x, m_RelativeScale.x)) *
		D2D1::Matrix3x2F::Rotation(m_RelativeRotation) *
		D2D1::Matrix3x2F::Translation(m_RelativeTranslation.x, m_RelativeTranslation.y);

	if (m_pParentScene != nullptr)
	{
		m_WorldTransform = m_RelativeTransform * m_pParentScene->m_WorldTransform;
	}
	else
	{
		m_WorldTransform = m_RelativeTransform;
	}
}
