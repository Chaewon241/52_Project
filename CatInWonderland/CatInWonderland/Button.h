#pragma once
#include "Object.h"

namespace catInWonderland
{
	enum class eButtonState
	{
		None,
		Hover,
		Click
	};

	class Button : public Object
	{
	public:
		Button(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite);
		virtual ~Button() = default;

		virtual void Update();

		inline eButtonState GetButtonState() const;

	private:
		eButtonState mButtonState;
	};

	eButtonState Button::GetButtonState() const
	{
		return mButtonState;
	}
}


