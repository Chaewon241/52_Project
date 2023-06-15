#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerPlummet.h"
#include "PlayerAction.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SpriteManager.h"
#include "BoardManager.h"
#include "eAnimaitonType.h"
#include "SpriteManager.h"

namespace catInWonderland
{
	PlayerIdle::PlayerIdle()
		: PlayerState(ePlayerState::Idle)
		, mIntervalTime(0.5f)
		, mElapsedTime(0.f)
		, mAnimationIndex(0u)
		, mAnimationRectangles(SpriteManager::GetInstance()->GetSpriteRectangles(eAnimationType::PlayerIdle))
	{
	}

	PlayerState* PlayerIdle::HandleStateOrNull(Player* player)
	{
		if (InputManager::GetInstance()->GetKeyState('A') == eKeyState::PUSH)	//Key�� ������
		{
			if (player->GetBoardX() != 0)
			{
				//�������� �� ĭ �̵�	
				player->SetIsLeft(true);
				player->BoardMove(-1, 0);
				player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerMove));
				player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerMove, false));
				return new PlayerMove();
			}
		}
		else if (InputManager::GetInstance()->GetKeyState('D') == eKeyState::PUSH)
		{
			if (player->GetBoardX() < BoardManager::GetInstance()->GetBoardSize() - 1)
			{
				player->SetIsLeft(false);
				player->BoardMove(+1, 0);
				player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerMove));
				player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerMove, false));
				return new PlayerMove();
			}
		}

		if (InputManager::GetInstance()->GetKeyState('W') == eKeyState::PUSH)
		{
			player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerAction));
			player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerAction, false));
			return new PlayerAction();
		}

		if (BoardManager::GetInstance()->GetBoardSize() - 1 != player->GetBoardY())
		{
			player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerPlummet));
			player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerPlummet, false));
			
			return new PlayerPlummet;
		}

		return nullptr;
	}

	void PlayerIdle::Update(Player* player)
	{
		const float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime();	//��Ÿ Ÿ��

		mElapsedTime += DELTA_TIME;		// ��� �ð� += ��Ÿ Ÿ��

		if (mIntervalTime < mElapsedTime)	//���� �������� �� ��� �ð��� �Ѿ��ٸ�
		{
			mElapsedTime -= mIntervalTime;	//�� ��� �ð� - �������� �� ó������ �ٽ� ���ư� �ִϸ��̼� �����
			mAnimationIndex = (mAnimationIndex + 1) % mAnimationRectangles.size();	// ��(Player �ִϸ��̼�)�� ���� ������(��)
			player->SetSpriteRectangle(mAnimationRectangles.at(mAnimationIndex));
		}
	}
}