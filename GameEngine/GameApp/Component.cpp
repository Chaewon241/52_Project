#include "pch.h"
#include "Component.h"
#include "Object.h"
#include "GameObject.h"
#include "D2DRenderer.h"

Component::Component(GameObject* gameObject)
	: m_GameObject(gameObject)
{
}

Component::~Component()
{
}

void Component::Update()
{
}

void Component::Render(D2DRenderer* renderer)
{
	
}

