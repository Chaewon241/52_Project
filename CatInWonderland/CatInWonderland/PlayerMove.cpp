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
		// 이동처리
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

		mAniElapsedTime += DELTA_TIME;		// 경과 시간 += 델타 타임

		if (mAniIntervalTime < mAniElapsedTime)	//만약 프레임이 총 경과 시간을 넘었다면
		{
			mAniElapsedTime -= mAniIntervalTime;	//총 경과 시간 - 프레임을 해 처음으로 다시 돌아가 애니메이션 재수행
			mAnimationIndex = (mAnimationIndex + 1) % mAnimationRectangles.size();	// 값(Player 애니메이션)의 나올 사이즈(값)
			player->SetSpriteRectangle(mAnimationRectangles.at(mAnimationIndex));
		}
	}
}
