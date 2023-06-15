#pragma once

#include "PlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerAction : public PlayerState
	{
	public:
		PlayerAction();
		~PlayerAction() = default;

		PlayerState* HandleStateOrNull(Player* player);
		void Update(Player* player);

	private:
		bool mActionCheck;
	};
}
