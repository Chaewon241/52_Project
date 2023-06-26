#include "pch.h"
#include "GameObject.h"
#include <vector>

void Update()
{

}

void Render()
{

}

GameObject::GameObject()
	: m_parent(nullptr)
	, m_childrenList({ 0 })
	, m_componentList({ 0 })
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(float deltatime)
{
}

void GameObject::Render(D2DRenderer* renderer)
{
}

void GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
	m_childrenList.push_back(child);
}
