#include <windows.h>

#include "Stage01.h"
#include "Player.h"
#include "BoardManager.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "WinApp.h"
#include "Tile.h"
#include "Brick.h"
#include "TimeManager.h"

namespace catInWonderland
{
	Stage01::Stage01()
		: GameScene(3)
	{

	}

	void Stage01::Enter()
	{
		mBoardSize = 10;
		GameScene::Enter();
		size_t boardX = 0;
		size_t boardY = 2;

		hRectangle worldRect = BoardManager::GetInstance()->GetWorldRectangle(boardX, boardY);
		mPlayer = new Player(worldRect, hRectangle(0, 0, 360, 360), &SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle), boardX, boardY);
		RegisterBoardObject(mPlayer);

		worldRect = BoardManager::GetInstance()->GetWorldRectangle(boardX, boardY);
		BoardObject* brick = new Brick(worldRect, hRectangle(0, 0, 360, 360), &SpriteManager::GetInstance()->GetSprite(eSpriteType::Brick), boardX, boardY);
		RegisterBoardObject(brick);
	}

	eSceneType Stage01::HandleScene()
	{
		return mSceneType;
	}
}