#pragma once

#include "ePlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerState
	{
	public:
		PlayerState(ePlayerState playerState);
		virtual	~PlayerState() = default;

		virtual PlayerState* HandleStateOrNull(Player* outPlayer) = 0;
		virtual void Update(Player* outPlayer) = 0;

		inline ePlayerState GetPlayerState() const;

	protected:
		ePlayerState mPlayerState;
	};

	ePlayerState PlayerState::GetPlayerState() const
	{
		return mPlayerState;
	}
}