#include "Button.h"
#include "InputManager.h"

namespace catInWonderland
{
	Button::Button(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite)
		: Object(worldRectangle, spriteRectangle, sprite)
		, mButtonState(eButtonState::None)
	{
	}

	void Button::Update()
	{
		const POINT& mousePos = InputManager::GetInstance()->GetMousePos();
		
		if (hRectangle::IsContainVertex(mWorldRectangle, mousePos.x, mousePos.y))
		{
			if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == eKeyState::PUSH)
			{
				mButtonState = eButtonState::Click;
			}
			else
			{
				mButtonState = eButtonState::Hover;
			}
		}
		else
		{
			mButtonState = eButtonState::None;
		}
	}
}
