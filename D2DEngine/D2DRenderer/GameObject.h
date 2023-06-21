#pragma once

#include "SceneComponent.h"
#include <vector>

class GameObject
{
private:
public:
	std::vector<Component>* m_OwnedComponent;
	SceneComponent* m_pRootComponent;

public:
	GameObject() {};
	~GameObject() {};

	void Init();
	void Update();
	void Render();
	void SetLocation();
	void GetLocation();

	template <typename T>
	T CreateComponent();
};