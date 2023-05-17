#pragma once

#include "BoardObject.h"

namespace catInWonderland
{
	class PlayerState;

	class Player : public BoardObject
	{
	public:
		Player(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		~Player() = default;

		virtual void Update();

		// void Animation(float deltaTime);
		// void Action();
	};
}