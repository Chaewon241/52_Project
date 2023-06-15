#include "PlayerPlummet.h"
#include "BoardManager.h"
#include "SpriteManager.h"
#include "TimeManager.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerIdle.h"


namespace catInWonderland
{
	PlayerPlummet::PlayerPlummet()
		:PlayerState(ePlayerState::Plummet)
		, mDistance(BoardManager::GetInstance()->GetCellSize())
		, mbLeft(false)
		, mElapsed(0.f)
		, mPlummetIntervalTime(0.5f)
		, mPlummetElapsedTime(0.f)
		, mPlummetmationIndex(0u)
		, mAnimationRectangles(SpriteManager::GetInstance()->GetSpriteRectangles(eAnimationType::PlayerPlummet))
	{
	}

	PlayerState* PlayerPlummet::HandleStateOrNull(Player* player)
	{
		int max = BoardManager::GetInstance()->GetBoardSize() - 1;
		int playerboard = player->GetBoardY();
		if (max == playerboard)
		{
			player->AlignWorldRectagle();
			player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle));
			player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerIdle, false));
			return new PlayerIdle;
		}
		// 아랫바닥에 벽돌이 있을 시에도 변경하는 로직 추가해야 됨

		return nullptr;
	}

	void PlayerPlummet::Update(Player* player)
	{
		const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime() / 0.2f;

		mPlummetElapsedTime += DELTA_TIME;		// 경과 시간 += 델타 타임

		if (mPlummetIntervalTime < mPlummetElapsedTime)	//만약 프레임이 총 경과 시간을 넘었다면
		{
			mPlummetElapsedTime -= mPlummetIntervalTime;	//총 경과 시간 - 프레임을 해 처음으로 다시 돌아가 애니메이션 재수행
			mPlummetmationIndex = (mPlummetmationIndex + 1) % mAnimationRectangles.size();	// 값(Player 애니메이션)의 나올 사이즈(값)
			player->SetSpriteRectangle(mAnimationRectangles.at(mPlummetmationIndex));
		}

		float downPlummet = mDistance * DELTA_TIME;
		mElapsed += downPlummet;

		if (BoardManager::GetInstance()->GetBoardSize() - 1 != player->GetBoardY())
		{
			// 다음 칸까지 이동했을 때
			if (mElapsed >= mDistance)
			{
				mElapsed -= mDistance;
				player->BoardMove(0, 1);
			}

			player->Move(0.f, downPlummet);
		}
	}
}
