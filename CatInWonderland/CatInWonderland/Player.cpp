#include <cassert>
#include <Windows.h>

#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdle.h"
#include "InputManager.h"
#include "BoardManager.h"

namespace catInWonderland
{
	Player::Player(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY)
		: BoardObject(worldRectangle, spriteRectangle, sprite, boardX, boardY)
		, mPlayerState(new PlayerIdle())
		, mbLeft(true)
	{
	}

	Player::~Player()
	{
		delete mPlayerState;
		mPlayerState = nullptr;
	}

	void Player::Update()
	{
		mPlayerState->Update(this);
		PlayerState* nextState = mPlayerState->HandleStateOrNull(this);

		if (nextState != nullptr)
		{
			delete mPlayerState;
			mPlayerState = nextState;
		}
	}

	//if (InputManager::GetInstance()->GetKeyState('A') == eKeyState::PUSH)
	//{
	//	if (mBoardX != 0)
	//	{
	//		--mBoardX;	//왼쪽으로 한 칸 이동
	//	}

	//	mWorldRectangle = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);	//현재 좌표 저장
	//}
	//else if (InputManager::GetInstance()->GetKeyState('D') == eKeyState::PUSH)
	//{
	//	const unsigned int BOARD_SIZE = BoardManager::GetInstance()->GetBoardSize();	//보드 최대 사이즈

	//	++mBoardX;
	//	if (mBoardX >= BOARD_SIZE)
	//	{
	//		mBoardX = BOARD_SIZE - 1;
	//	}

	//	mWorldRectangle = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);
	//}

	void Player::RotateBoardIndex(bool bLeft)	//캐릭터 회전으로 회전을 해도 본 상태를 유지함
	{
		BoardObject::RotateBoardIndex(bLeft);
		const Vector2 PLAYER_CENTER = mWorldRectangle.GetCenter();
		const float RANDIAN = 1.57079f;

		if (bLeft)
		{
			Rotate(PLAYER_CENTER.GetX(), PLAYER_CENTER.GetY(), RANDIAN);
		}
		else
		{
			Rotate(PLAYER_CENTER.GetX(), PLAYER_CENTER.GetY(), -RANDIAN);
		}

		CheckFloor();
	}

	void Player::CheckFloor()	//떨어지기
	{
		mBoardY = BoardManager::GetInstance()->GetBoardSize() - 1;
		mWorldRectangle = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);
	}

	void Player::Move(float x, float y)
	{
		mWorldRectangle.Move(x, y);
	}

	//void Player::Animation(float deltaTime)
	//{

	//}

	//void Player::Action()
	//{

	//}
}