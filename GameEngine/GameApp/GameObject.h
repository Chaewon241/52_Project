#pragma once
#include "Object.h"
#include "D2DRenderer.h"

#include <vector>

class Component;

class GameObject : public Object
{
private:
	GameObject* m_parent;
	std::vector<GameObject*> m_childrenList;
	std::vector<Component*> m_componentList;
public:
	GameObject();
	virtual ~GameObject();
public:
	virtual void Update(float deltatime) override;
	virtual void Render(D2DRenderer* renderer) override;
public:
	void AddChild(GameObject* child);
	void SetParent(GameObject* parent) { m_parent = parent; }
	GameObject* GetParent() { return m_parent; }
public:
	template <typename TComponent>
	void AddComponent();
	template <typename TComponent>
	TComponent* GetComponent();
};

template<typename TComponent>
inline void GameObject::AddComponent()
{
	TComponent* _newComponent = new TComponent(this);
	m_componentList.push_back(_newComponent);
}

template<typename TComponent>
inline TComponent* GameObject::GetComponent()
{
	for (auto& component : m_componentList)
	{
		TComponent* tcom = dynamic_cast<TComponent*>(component);
		if (tcom != nullptr)
			return tcom;
	}
	return nullptr;
}

