#pragma once
#include "BoardObject.h"

namespace catInWonderland	//��� �׸���
{
	class Tile : public BoardObject
	{
	public:
		Tile(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		~Tile() = default;

		virtual void Update();
	};
}