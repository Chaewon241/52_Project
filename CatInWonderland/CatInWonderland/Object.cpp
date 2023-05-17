#include "Object.h"
#include "RenderManager.h"

namespace catInWonderland
{
	Object::Object(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite)
		: mWorldRectangle(worldRectangle)
		, mSpriteRectangle(spriteRectangle)
		, mSprite (sprite)
	{
	}

	void Object::Render()
	{
		RenderManager::GetInstance()->Draw(mWorldRectangle, mSpriteRectangle, *mSprite);
	}
}