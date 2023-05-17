#pragma once

#include "PlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerMove : public PlayerState
	{
	public:
		virtual PlayerState* HandleStateOrNull(const Player& player);
		virtual void Update(Player* outPlayer);
	};
}