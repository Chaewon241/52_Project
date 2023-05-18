#include "PlayerMove.h"
#include "PlayerIdle.h"
#include "Player.h"
#include "BoardManager.h"
#include "SpriteManager.h"
#include "TimeManager.h"
#include "InputManager.h"

namespace catInWonderland
{
	PlayerMove::PlayerMove()
		: PlayerState(ePlayerState::Move)
		, mDistance(BoardManager::GetInstance()->GetCellSize())
		, mElapsed(0.f)
		, mAniIntervalTime(0.05f)
		, mAniElapsedTime(0.f)
		, mAnimationIndex(0u)
		, mAnimationRectangles(SpriteManager::GetInstance()->GetSpriteRectangles(eAnimationType::PlayerMove))
	{
	}

	PlayerState* PlayerMove::HandleStateOrNull(Player* player)
	{
		if (mElapsed >= 1.0f)
		{
			/*float remainderDistance = (mElapsed - 1.0f) * mDistance;

			if (player->GetIsLeft())
			{
				player->Move(remainderDistance, 0.f);
			}
			else
			{
				player->Move(-remainderDistance, 0.f);
			}*/

			player->AlignWorldRectagle();
			player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle));
			player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerIdle, false));

			return new PlayerIdle;
		}

		return nullptr;
	}

	void PlayerMove::Update(Player* player)
	{
		// �̵�ó��
		const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime() / 0.3f;

		float currentDistance = mDistance * DELTA_TIME;
		mElapsed += DELTA_TIME;

		if (player->GetIsLeft())
		{
			player->Move(-currentDistance, 0.f);
		}
		else
		{
			player->Move(currentDistance, 0.f);
		}

		mAniElapsedTime += DELTA_TIME;		// ��� �ð� += ��Ÿ Ÿ��

		if (mAniIntervalTime < mAniElapsedTime)	//���� �������� �� ��� �ð��� �Ѿ��ٸ�
		{
			mAniElapsedTime -= mAniIntervalTime;	//�� ��� �ð� - �������� �� ó������ �ٽ� ���ư� �ִϸ��̼� �����
			mAnimationIndex = (mAnimationIndex + 1) % mAnimationRectangles.size();	// ��(Player �ִϸ��̼�)�� ���� ������(��)
			player->SetSpriteRectangle(mAnimationRectangles.at(mAnimationIndex));
		}
	}
}
