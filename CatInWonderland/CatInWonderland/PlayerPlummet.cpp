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
		// �Ʒ��ٴڿ� ������ ���� �ÿ��� �����ϴ� ���� �߰��ؾ� ��

		return nullptr;
	}

	void PlayerPlummet::Update(Player* player)
	{
		const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime() / 0.2f;

		mPlummetElapsedTime += DELTA_TIME;		// ��� �ð� += ��Ÿ Ÿ��

		if (mPlummetIntervalTime < mPlummetElapsedTime)	//���� �������� �� ��� �ð��� �Ѿ��ٸ�
		{
			mPlummetElapsedTime -= mPlummetIntervalTime;	//�� ��� �ð� - �������� �� ó������ �ٽ� ���ư� �ִϸ��̼� �����
			mPlummetmationIndex = (mPlummetmationIndex + 1) % mAnimationRectangles.size();	// ��(Player �ִϸ��̼�)�� ���� ������(��)
			player->SetSpriteRectangle(mAnimationRectangles.at(mPlummetmationIndex));
		}

		float downPlummet = mDistance * DELTA_TIME;
		mElapsed += downPlummet;

		if (BoardManager::GetInstance()->GetBoardSize() - 1 != player->GetBoardY())
		{
			// ���� ĭ���� �̵����� ��
			if (mElapsed >= mDistance)
			{
				mElapsed -= mDistance;
				player->BoardMove(0, 1);
			}

			player->Move(0.f, downPlummet);
		}
	}
}
