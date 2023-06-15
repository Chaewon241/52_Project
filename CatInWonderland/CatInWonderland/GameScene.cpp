#include <windows.h>

#include "GameScene.h"
#include "Player.h"
#include "BoardManager.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "WinApp.h"
#include "Tile.h"
#include "TimeManager.h"


namespace catInWonderland
{
	GameScene::GameScene(unsigned int boardSize)
		:mBoardSize(boardSize)
	{

	}

	void GameScene::Enter()
	{
		mElapsedTime = 0.f;
		mbLeft = true;
		mbRotate = false;

		BoardManager::GetInstance()->CreateBoard(mBoardSize, 100, 0);

		hRectangle worldRect;
		BoardObject* boardObject;

		for (int i = 0; i < mBoardSize; ++i)
		{
			for (int j = 0; j < mBoardSize; ++j)
			{
				worldRect = BoardManager::GetInstance()->GetWorldRectangle(i, j);
				boardObject = new Tile(worldRect, hRectangle(0, 0, 393, 183), &SpriteManager::GetInstance()->GetSprite(eSpriteType::Tile), i, j);
				RegisterBoardObject(boardObject);
			}
		}
	}

	void GameScene::Update()
	{
		if (mbRotate && mPlayer->GetPlayerState() == ePlayerState::Idle)
		{
			const UINT CENTER_X = WinApp::GetWidth() / 2;
			const UINT CENTER_Y = WinApp::GetHeight() / 2;
			const float RADIAN = 1.57079f;

			const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime();
			const float WEIGHT = DELTA_TIME * RADIAN * (mbLeft ? -1 : 1);
			mElapsedTime += fabs(WEIGHT);

			for (auto iter = mBoardObjects.begin(); iter != mBoardObjects.end(); ++iter)
			{
				(*iter)->Rotate(CENTER_X, CENTER_Y, WEIGHT);
			}

			// 이 영역은 매프레임 델타타임으로 인한 가중치로 처리되는 공간

			if (mElapsedTime >= RADIAN)
			{
				// 이 영역은 회전이 끝나고 남은 변수를 초기화하고 정렬해주는 공간
				mbRotate = false;
				const float REMAINDER = (mElapsedTime - RADIAN) * (mbLeft ? 1 : -1);
				mElapsedTime = 0;

				for (auto iter = mBoardObjects.begin(); iter != mBoardObjects.end(); ++iter)
				{
					(*iter)->Rotate(CENTER_X, CENTER_Y, REMAINDER);
					(*iter)->RotateBoardIndex(mbLeft);
					(*iter)->AlignWorldRectagle();
				}
			}

			return;
		}

		Scene::Update();
		if (InputManager::GetInstance()->GetKeyState('Q') == eKeyState::PUSH)
		{
			mbRotate = true;
			mbLeft = true;
		}
		else if (InputManager::GetInstance()->GetKeyState('E') == eKeyState::PUSH)
		{
			mbRotate = true;
			mbLeft = false;
		}
	}

	void GameScene::Exit()
	{
		Scene::Exit();
		mPlayer = nullptr;
	}
}

