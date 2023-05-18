#include "Magent.h"
#include "InputManager.h"
#include "BoardManager.h"

namespace catInWonderland
{
	Magnet::Magnet(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY, hRectangle magneObject)
		: BoardObject(worldRectangle, spriteRectangle, sprite, boardX, boardY)
		, mMagneObject(magneObject)
	{
	}

	void Magnet::Update()
	{

	}

	void Magnet::MagnetObject()
	{
		mMagneObject = BoardManager::GetInstance()->GetWorldRectangle(mBoardX, mBoardY);
	}
}
