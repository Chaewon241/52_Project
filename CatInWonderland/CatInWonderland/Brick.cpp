#include <cmath>

#include "Brick.h"
#include "InputManager.h"
#include "BoardManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Player.h"
#include "Scene.h"

namespace catInWonderland
{
	Brick::Brick(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY)
		: BoardObject(worldRectangle, spriteRectangle, sprite, eObjectType::Brick, boardX, boardY)
		, mDiffX(boardX)
		, mDiffY(boardY)
		, mbAction(false)
		, mbFrame(false)
		, mElapsedX(0.f)
		, mElapsedY(0.f)
		, mMagentState(eMagent::Blue)
	{
	}

	void Brick::Update()
	{
		if (mbAction)
		{
			const std::vector<Object*>& objects = SceneManager::GetInstance()->GetCurrentScene().GetObjects();

			for (auto iter = objects.begin(); iter != objects.end(); ++iter)	//반복자
			{
				Object* obj = (*iter);

				if (obj->GetObjectType() == eObjectType::Player)	//타입이 오브젝트라면
				{
					// 맴버 함수 내부에서 맴버 변수에 접근하는 것은
					// 함수를 호출한 개체의 인스턴스의 맴버에 접근하는 것과 같다.
					// mWorldRectangle == this->mWorldRectangle;
					Vector2 playerCenter = obj->GetWorldRectangle().GetCenter();
					Vector2 currentCenter = mWorldRectangle.GetCenter();

					// 델타타임에 의해서 이동하기 위해서 맴버변수

					mDiffX = playerCenter.GetX() - currentCenter.GetX();	//바로 끌어옴
					mDiffY = playerCenter.GetY() - currentCenter.GetY();
				}
			}

			mbAction = false;
			mbFrame = true;
		}

		if (mbFrame)
		{
			const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime();
			float curDistanceX = mDiffX * DELTA_TIME;
			float curDistanceY = mDiffY * DELTA_TIME;

			mElapsedX += curDistanceX;
			mElapsedY += curDistanceY;

			Move(curDistanceX, curDistanceY);

			if (mDiffX <= mElapsedX && mDiffY <= mElapsedY)
			{
				const float REMANIDER_X = mDiffX - mElapsedX;
				const float REMANIDER_Y = mDiffY - mElapsedY;

				Move(REMANIDER_X, REMANIDER_Y);

				const float CELL_SIZE = BoardManager::GetInstance()->GetCellSize();
				BoardMove(roundf(mDiffX / CELL_SIZE), roundf(mDiffY / CELL_SIZE));	//한 칸의 크기 구하기

				mbFrame = false;
				mDiffX = 0.f;
				mDiffY = 0.f;
				mElapsedX = 0.f;
				mElapsedY = 0.f;
			}
		}
	}

	void Brick::Action()
	{
		mbAction = true;
	}
}