#include "pch.h"
#include "UIManager.h"

void UIManager::Update(float deltaTime)
{
	UpdateHoveredUI(deltaTime);
	UpdateClickedUI(deltaTime);
	ClearFocusedUI();
}

void UIManager::ClearFocusedUI()
{
	m_HoveredUIObjects.clear();
	m_ClickedUIObjects.clear();
}

void UIManager::UpdateHoveredUI(float deltaTime)
{
	for (auto& UI : m_HoveredUIObjects)
	{
		UI.lock()->ExcuteHoverFunctors(deltaTime);
	}
}

void UIManager::UpdateClickedUI(float deltaTime)
{
	for (auto& UI : m_ClickedUIObjects)
	{
		UI.lock()->ExcuteClickFunctors(deltaTime);
	}
}

void UIManager::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	int mouseX = MouseState.x;
	int mouseY = MouseState.y;

	for (auto& UI : m_UIObjects)
	{
		Math::Vector2 pos = UI->GetPosition();
		Math::Vector2 dpos = UI->GetSize() / 2.f;

		if (mouseX >= pos.x - dpos.x &&
			mouseY >= pos.y - dpos.y &&
			mouseX <= pos.x + dpos.x &&
			mouseY <= pos.y + dpos.y)
		{
			m_HoveredUIObjects.push_back(UI);

			if (MouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_ClickedUIObjects.push_back(UI);
			}
		}
	}
}
