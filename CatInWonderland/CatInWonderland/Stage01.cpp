#include <windows.h>

#include "Stage01.h"
#include "Player.h"
#include "BoardManager.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "WinApp.h"
#include "Tile.h"

namespace catInWonderland
{

	void Stage01::Enter()
	{
		const int BOARD_SIZE = 3;

		BoardManager::GetInstance()->SetBoardSize(BOARD_SIZE);
		BoardManager::GetInstance()->Apply();

		size_t boardX = 0;
		size_t boardY = 2;

		hRectangle worldRect;
		BoardObject* boardObject;

		for (int i = 0; i < BOARD_SIZE; ++i)
		{
			for (int j = 0; j < BOARD_SIZE; ++j)
			{
				worldRect = BoardManager::GetInstance()->GetWorldRectangle(i, j);
				boardObject = new Tile(worldRect, hRectangle(0, 0, 393, 183), &SpriteManager::GetInstance()->GetSprite(eSpriteType::Tile), i, j);
				RegisterBoardObject(boardObject);
			}
		}

		worldRect = BoardManager::GetInstance()->GetWorldRectangle(boardX, boardY);
		boardObject = new Player(worldRect, hRectangle(0, 0, 360, 360), &SpriteManager::GetInstance()->GetSprite(eSpriteType::Player), boardX, boardY);
		RegisterBoardObject(boardObject);
	}

	eSceneType Stage01::HandleScene()
	{
		return mSceneType;
	}

	void Stage01::Update()
	{
		Scene::Update();

		const UINT CENTER_X = WinApp::GetWidth() / 2;
		const UINT CENTER_Y = WinApp::GetHeight() / 2;
		const float RADIAN = 1.57079f;

		if (InputManager::GetInstance()->GetKeyState('Q') == eKeyState::PUSH)
		{
			for (auto iter = mBoardObjects.begin(); iter != mBoardObjects.end(); ++iter)
			{
				(*iter)->Rotate(CENTER_X, CENTER_Y, -RADIAN);
				(*iter)->RotateBoardIndex(true);
				(*iter)->AlignWorldRectagle();
			}
		}
		else if (InputManager::GetInstance()->GetKeyState('E') == eKeyState::PUSH)
		{
			for (auto iter = mBoardObjects.begin(); iter != mBoardObjects.end(); ++iter)
			{
				(*iter)->Rotate(CENTER_X, CENTER_Y, RADIAN);
				(*iter)->RotateBoardIndex(false);
				(*iter)->AlignWorldRectagle();
			}
		}
	}

	void Stage01::Exit()
	{
		Scene::Exit();
		mPlayer = nullptr;
	}
}