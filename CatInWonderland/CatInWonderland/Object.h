#pragma once

#include "hRectangle.h"

namespace catInWonderland
{
	class Sprite;

	class Object
	{
	public:
		Object(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite);
		~Object() = default;

		virtual void Update() = 0;
		virtual void Render();

	protected:
		hRectangle mWorldRectangle;
		hRectangle mSpriteRectangle;
		Sprite* mSprite;
	};
}