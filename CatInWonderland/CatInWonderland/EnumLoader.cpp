#include "EnumLoader.h"
#include "SpriteManager.h"
#include "eSpriteType.h"

namespace catInWonderland
{
	void EnumLoader::Load()
	{
		LoadSprtie();
		LoadAnimationRectangle();
	}

	void EnumLoader::LoadSprtie()
	{
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerIdle, L".\\resources\\idle_trans.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerMove, L".\\resources\\walk_sprite.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::Tile, L".\\resources\\blue_brick.bmp");
	}

	void EnumLoader::LoadAnimationRectangle()
	{
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerIdle, L".\\resources\\player_idle.txt");
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerMove, L".\\resources\\player_Move.txt");
	}
}