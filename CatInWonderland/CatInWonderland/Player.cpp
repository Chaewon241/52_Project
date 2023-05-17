#include <cassert>
#include <Windows.h>

#include "Player.h"
#include "InputManager.h"
#include "BoardManager.h"


namespace catInWonderland
{
	Player::Player(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY)
		: BoardObject(worldRectangle, spriteRectangle, sprite, boardX, boardY)
	{
	}

	void Player::Update()
	{
		if (InputManager::GetInstance()->GetKeyState('A') == eKeyState::PUSH)
		{
			if (mBoardX != 0)
			{
				--mBoardX;	//�������� �� ĭ �̵�
			}

			mWorldRectangle = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);	//���� ��ǥ ����
		}
		else if (InputManager::GetInstance()->GetKeyState('D') == eKeyState::PUSH)
		{
			const unsigned int BOARD_SIZE = BoardManager::GetInstance()->GetBoardSize();	//���� �ִ� ������

			++mBoardX;
			if (mBoardX >= BOARD_SIZE)
			{
				mBoardX = BOARD_SIZE - 1;
			}

			mWorldRectangle = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);
		}
	}

	//void Player::Animation(float deltaTime)
	//{

	//}

	//void Player::Action()
	//{

	//}
}