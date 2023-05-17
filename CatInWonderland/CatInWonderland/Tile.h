#pragma once
#include "BoardObject.h"

namespace catInWonderland	//배경 그리기
{
	class Tile : public BoardObject
	{
	public:
		Tile(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		~Tile() = default;

		virtual void Update();
	};
}