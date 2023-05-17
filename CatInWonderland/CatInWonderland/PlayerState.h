#pragma once

#include "ePlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerState
	{
	public:
		virtual PlayerState* HandleStateOrNull(const Player& player) = 0;
		virtual void Update(Player* outPlayer) = 0;

	protected:
		ePlayerState mPlayerState;
	};
}