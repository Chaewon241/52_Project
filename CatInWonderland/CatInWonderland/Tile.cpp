#include "Tile.h"

namespace catInWonderland
{
	Tile::Tile(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY)
		: BoardObject(worldRectangle, spriteRectangle, sprite, eObjectType::None, boardX, boardY)
	{
	}

	void Tile::Update()
	{

	}
}

