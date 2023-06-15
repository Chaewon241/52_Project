#include "EnumLoader.h"
#include "SpriteManager.h"
#include "eSpriteType.h"
#include "SceneLoader.h"
#include "SoundManager.h"

namespace catInWonderland
{
	void EnumLoader::Load()
	{
		LoadSprtie();
		LoadAnimationRectangle();
		SceneLoader::Load();
		LoadSound();
	}

	void EnumLoader::LoadSprtie()
	{
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerIdle, L".\\resources\\idle_trans.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerMove, L".\\resources\\walk_sprite.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerPlummet, L".\\resources\\sprite.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::PlayerAction, L".\\resources\\sprite.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::Tile, L".\\resources\\tile.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::Brick, L".\\resources\\blue_brick.bmp");
	}

	void EnumLoader::LoadAnimationRectangle()
	{
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerIdle, L".\\resources\\player_idle.txt");
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerMove, L".\\resources\\player_Move.txt");
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerPlummet, L".\\resources\\player_Plummet.txt");
		SpriteManager::GetInstance()->LoadAnimationRectangle(eAnimationType::PlayerAction, L".\\resources\\player_Plummet.txt");
	}

	void EnumLoader::LoadSound()
	{
		SoundManager::GetInstance()->LoadMusic(eSoundList::BGM1, ".\\music\\BGM1.mp3");
		SoundManager::GetInstance()->LoadMusic(eSoundList::BGM2, ".\\music\\BGM2.mp3");
		// 이건 테스트
		//SoundManager::GetInstance()->PlayMusic(eSoundList::BGM2, eSoundChannel::BGM, 0.5f);
	}
}