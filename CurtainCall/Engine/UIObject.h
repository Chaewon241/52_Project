#pragma once
#include "UIMouseHoverFunctor.h"
#include "UIMouseClickFunctor.h"

//enum class eUIType
//{
//    Button,
//
//    END
//};

class UIObject
{
public:
	UIObject(std::string name);
	virtual ~UIObject();

public:
	const Math::Vector2& GetPosition() const { return m_position; }
	const Math::Vector2& GetSize() const { return m_size; }

public:
	void SetMouseOverFunctor(std::weak_ptr<UIMouseHoverFunctor> functor) { m_mouseHoverFunctors.push_back(functor.lock()); }
	void SetMouseClickFunctor(std::weak_ptr<UIMouseClickFunctor> functor) { m_mouseClickFunctors.push_back(functor.lock()); }

	void SetPosition(const Math::Vector2& position);
	void SetSize(const Math::Vector2& size);

public:
	void AddChild(std::shared_ptr<UIObject> child);
	void RemoveChild(std::shared_ptr<UIObject> child);

	void ExcuteHoverFunctors(float deltaTime);
	void ExcuteClickFunctors(float deltaTime);

private:
	std::string m_name;

	std::vector<std::shared_ptr<UIMouseHoverFunctor>> m_mouseHoverFunctors;
	std::vector<std::shared_ptr<UIMouseClickFunctor>> m_mouseClickFunctors;

	bool m_bVisible;

	Math::Vector2 m_position;
	Math::Vector2 m_size;

	std::vector<std::shared_ptr<UIObject>> m_children;
};

