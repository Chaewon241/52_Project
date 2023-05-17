#include <cassert>

#include "InputManager.h"
#include "BoardManager.h"
#include "WinApp.h"
#include "BoardObject.h"

namespace catInWonderland
{
	BoardObject::BoardObject(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY)
		: Object(worldRectangle, spriteRectangle, sprite)
		, mBoardX(boardX)
		, mBoardY(boardY)
	{
	}

	void BoardObject::Rotate(int originX, int originY, float radian)
	{
		mWorldRectangle.Rotate(originX, originY, radian);
	}

	void BoardObject::RotateBoardIndex(bool bLeft)
	{
		if (bLeft)
		{
			BoardManager::GetInstance()->RotateLeft(&mBoardX, &mBoardY);
		}
		else
		{
			BoardManager::GetInstance()->RotateRight(&mBoardX, &mBoardY);
		}
	}

	void BoardObject::AlignWorldRectagle()	//다시 정렬하는 함수
	{
		BoardManager::GetInstance()->AlignRectangle(mBoardX, mBoardY, &mWorldRectangle);
	}
}
