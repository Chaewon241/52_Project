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

			for (auto iter = objects.begin(); iter != objects.end(); ++iter)	//�ݺ���
			{
				Object* obj = (*iter);

				if (obj->GetObjectType() == eObjectType::Player)	//Ÿ���� ������Ʈ���
				{
					// �ɹ� �Լ� ���ο��� �ɹ� ������ �����ϴ� ����
					// �Լ��� ȣ���� ��ü�� �ν��Ͻ��� �ɹ��� �����ϴ� �Ͱ� ����.
					// mWorldRectangle == this->mWorldRectangle;
					Vector2 playerCenter = obj->GetWorldRectangle().GetCenter();
					Vector2 currentCenter = mWorldRectangle.GetCenter();

					// ��ŸŸ�ӿ� ���ؼ� �̵��ϱ� ���ؼ� �ɹ�����

					mDiffX = playerCenter.GetX() - currentCenter.GetX();	//�ٷ� �����
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
				BoardMove(roundf(mDiffX / CELL_SIZE), roundf(mDiffY / CELL_SIZE));	//�� ĭ�� ũ�� ���ϱ�

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