#include "Object.h"
#include "RenderManager.h"

namespace catInWonderland
{
	Object::Object(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, eObjectType objectType)
		: mWorldRectangle(worldRectangle)
		, mSpriteRectangle(spriteRectangle)
		, mSprite(sprite)
		, mObjectType(objectType)
	{
	}

	Object::~Object()
	{
		// haha
	}

	void Object::Render()
	{
		RenderManager::GetInstance()->Draw(mWorldRectangle, mSpriteRectangle, *mSprite);
	}
}