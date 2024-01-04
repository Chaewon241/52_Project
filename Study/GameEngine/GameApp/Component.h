#pragma once

class GameObject;
class D2DRenderer;
class AnimationClip;

class Component
{
protected:
	GameObject* m_GameObject;
public:
	Component(GameObject* gameObject);
	virtual ~Component();
	
public:
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
	GameObject* GetGameObject() const { return m_GameObject; }
	void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; }
};

