#pragma once

#include "hRectangle.h"
#include "eObjectType.h"
#include "ePlayerState.h"

namespace catInWonderland
{
	class Sprite;

	class Object
	{
	public:
		Object(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, eObjectType objectType);
		virtual ~Object();

		virtual void Update() = 0;
		virtual void Render();

		inline void Move(float x, float y);

		inline void SetSprite(Sprite* sprite);
		inline void SetSpriteRectangle(const hRectangle& spriteRectangle);

		inline const hRectangle& GetWorldRectangle() const;
		inline eObjectType GetObjectType() const;

	protected:
		hRectangle mWorldRectangle;
		hRectangle mSpriteRectangle;
		Sprite* mSprite;
		eObjectType mObjectType;
	};

	void Object::SetSprite(Sprite* sprite)
	{
		mSprite = sprite;
	}

	void Object::SetSpriteRectangle(const hRectangle& spriteRectangle)
	{
		mSpriteRectangle = spriteRectangle;
	}

	const hRectangle& Object::GetWorldRectangle() const
	{
		return mWorldRectangle;
	}

	eObjectType Object::GetObjectType() const
	{
		return mObjectType;
	}

	void Object::Move(float x, float y)
	{
		mWorldRectangle.Move(x, y);
	}
}