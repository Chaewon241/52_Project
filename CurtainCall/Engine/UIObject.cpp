#include "pch.h"
#include "UIObject.h"
#include "InputManager.h"

UIObject::UIObject(std::string name)
	: m_name(std::move(name))
{
}

UIObject::~UIObject()
{
}

void UIObject::SetPosition(const Math::Vector2& position)
{
	m_position = position;
}

void UIObject::SetSize(const Math::Vector2& size)
{
	m_size = size;
}

void UIObject::ExcuteHoverFunctors(float deltaTime)
{
	for (auto& functor : m_mouseHoverFunctors)
	{
		functor->Execute(*this, deltaTime);
	}
}

void UIObject::ExcuteClickFunctors(float deltaTime)
{
	for (auto& functor : m_mouseClickFunctors)
	{
		functor->Execute(*this, deltaTime);
	}
}

void UIObject::AddChild(std::shared_ptr<UIObject> child)
{
	m_children.push_back(child);
}

void UIObject::RemoveChild(std::shared_ptr<UIObject> child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);

	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}

//void UIObject::Update(float deltaTime)
//{
//	for (auto& child : m_children)
//	{
//		child->Update(deltaTime);
//	}
//}
