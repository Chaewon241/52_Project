#include "pch.h"
#include "GameObject.h"
#include "Component.h"

#include <vector>

enum State
{
	Idle,
	Move,
	Attack
};

void Update()
{

}

void Render()
{

}

GameObject::GameObject()
	: m_parent(nullptr)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (Component* cp : m_componentList)
	{
		cp->Update();
	}
}

void GameObject::Render(D2DRenderer* renderer)
{
	for (Component* cp : m_componentList)
	{
		cp->Render(renderer);
	}
}

// 삭제도 해줘야함

void GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
	m_childrenList.push_back(child);
}
