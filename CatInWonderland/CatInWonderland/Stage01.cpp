#include <windows.h>

#include "Stage01.h"
#include "Player.h"
#include "BoardManager.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "WinApp.h"
#include "Tile.h"
#include "TimeManager.h"

namespace catInWonderland
{
	void Stage01::Enter()
	{
		mElapsedTime = 0.f;
		mbLeft = true;
		mbRotate = false;
		const int BOARD_SIZE = 10;
		

		BoardManager::GetInstance()->CreateBoard(BOARD_SIZE, 100, 0);
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
		mPlayer = new Player(worldRect, hRectangle(0, 0, 360, 360), &SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle), boardX, boardY);
		RegisterBoardObject(mPlayer);
	}

	eSceneType Stage01::HandleScene()
	{

		return mSceneType;
	}

	void Stage01::Update()
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

	void Stage01::Exit()
	{
		Scene::Exit();
		mPlayer = nullptr;
	}
}