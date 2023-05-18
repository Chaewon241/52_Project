#pragma once

#include "PlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerAction : public PlayerState
	{

	public:
		PlayerState* HandleState(const Player& player) {};
		void Update(Player* player) {};
	};
}
