#pragma once

#include "hRectangle.h"

namespace catInWonderland
{
	class Sprite;

	class Object
	{
	public:
		Object(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite);
		virtual ~Object(); 

		virtual void Update() = 0;
		virtual void Render();

		inline void SetSprite(Sprite* sprite);
		inline void SetSpriteRectangle(const hRectangle& spriteRectangle);

	protected:
		hRectangle mWorldRectangle;
		hRectangle mSpriteRectangle;
		Sprite* mSprite;
	};

	void Object::SetSprite(Sprite* sprite)
	{
		mSprite = sprite;
	}

	void Object::SetSpriteRectangle(const hRectangle& spriteRectangle)
	{
		mSpriteRectangle = spriteRectangle;
	}
}